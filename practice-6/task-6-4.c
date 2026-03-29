#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// Типы сообщений
typedef enum {
    MSG_TEXT,
    MSG_IMAGE,
    MSG_FILE,
    MSG_LOCATION,
    MSG_AUDIO
} MessageType;

// Текстовое сообщение
typedef struct {
    char content[500];
    int is_edited;
} TextMessage;

// Изображение
typedef struct {
    char filename[100];
    int width;
    int height;
    size_t size_bytes;
} ImageMessage;

// Файл
typedef struct {
    char filename[100];
    char mime_type[50];
    size_t size_bytes;
} FileMessage;

// Локация
typedef struct {
    double latitude;
    double longitude;
    char address[200];
} LocationMessage;

// Аудио
typedef struct {
    char filename[100];
    int duration_seconds;
    size_t size_bytes;
} AudioMessage;

// Сообщение с tagged union
typedef struct {
    unsigned int id;
    unsigned int sender_id;
    unsigned int chat_id;
    time_t timestamp;
    MessageType type;
    union {
        TextMessage text;
        ImageMessage image;
        FileMessage file;
        LocationMessage location;
        AudioMessage audio;
    } content;
} Message;

//сообщения

Message create_text_message(unsigned int sender, unsigned int chat, const char *text) {
    Message msg = {0};
    msg.id = rand() % 100000;
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_TEXT;
    strncpy(msg.content.text.content, text, sizeof(msg.content.text.content) - 1);
    msg.content.text.is_edited = 0;
    return msg;
}

Message create_image_message(unsigned int sender, unsigned int chat,
                             const char *filename, int w, int h, size_t size) {
    Message msg = {0};
    msg.id = rand() % 100000;
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_IMAGE;
    strncpy(msg.content.image.filename, filename, sizeof(msg.content.image.filename) - 1);
    msg.content.image.width = w;
    msg.content.image.height = h;
    msg.content.image.size_bytes = size;
    return msg;
}

Message create_file_message(unsigned int sender, unsigned int chat,
                            const char *filename, const char *mime, size_t size) {
    Message msg = {0};
    msg.id = rand() % 100000;
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_FILE;
    strncpy(msg.content.file.filename, filename, sizeof(msg.content.file.filename) - 1);
    strncpy(msg.content.file.mime_type, mime, sizeof(msg.content.file.mime_type) - 1);
    msg.content.file.size_bytes = size;
    return msg;
}

Message create_location_message(unsigned int sender, unsigned int chat,
                                double lat, double lon, const char *address) {
    Message msg = {0};
    msg.id = rand() % 100000;
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_LOCATION;
    msg.content.location.latitude = lat;
    msg.content.location.longitude = lon;
    strncpy(msg.content.location.address, address, sizeof(msg.content.location.address) - 1);
    return msg;
}

Message create_audio_message(unsigned int sender, unsigned int chat,
                             const char *filename, int duration, size_t size) {
    Message msg = {0};
    msg.id = rand() % 100000;
    msg.sender_id = sender;
    msg.chat_id = chat;
    msg.timestamp = time(NULL);
    msg.type = MSG_AUDIO;
    strncpy(msg.content.audio.filename, filename, sizeof(msg.content.audio.filename) - 1);
    msg.content.audio.duration_seconds = duration;
    msg.content.audio.size_bytes = size;
    return msg;
}

// Сериализация 

size_t message_serialize(const Message *msg, unsigned char *buffer, size_t buffer_size) {
    if (!msg || !buffer) return 0;

    size_t offset = 0;
    size_t content_size = 0;

    // Определяем размер контента в зависимости от типа
    switch (msg->type) {
        case MSG_TEXT:
            content_size = sizeof(TextMessage);
            break;
        case MSG_IMAGE:
            content_size = sizeof(ImageMessage);
            break;
        case MSG_FILE:
            content_size = sizeof(FileMessage);
            break;
        case MSG_LOCATION:
            content_size = sizeof(LocationMessage);
            break;
        case MSG_AUDIO:
            content_size = sizeof(AudioMessage);
            break;
        default:
            return 0;
    }

    size_t total_size = 4 + 4 + 4 + 8 + 4 + 8 + content_size;
    if (total_size > buffer_size) return 0;

    // Копируем заголовок
    memcpy(buffer + offset, &msg->id, 4); offset += 4;
    memcpy(buffer + offset, &msg->sender_id, 4); offset += 4;
    memcpy(buffer + offset, &msg->chat_id, 4); offset += 4;
    memcpy(buffer + offset, &msg->timestamp, 8); offset += 8;
    memcpy(buffer + offset, &msg->type, 4); offset += 4;
    memcpy(buffer + offset, &content_size, sizeof(content_size)); offset += sizeof(content_size);

    // Копируем контент
    memcpy(buffer + offset, &msg->content, content_size);
    offset += content_size;

    return offset;
}

//Десериализация 

int message_deserialize(const unsigned char *buffer, size_t size, Message *msg) {
    if (!buffer || !msg || size < 32) return -1;

    size_t offset = 0;

    memcpy(&msg->id, buffer + offset, 4); offset += 4;
    memcpy(&msg->sender_id, buffer + offset, 4); offset += 4;
    memcpy(&msg->chat_id, buffer + offset, 4); offset += 4;
    memcpy(&msg->timestamp, buffer + offset, 8); offset += 8;
    memcpy(&msg->type, buffer + offset, 4); offset += 4;

    size_t content_size;
    memcpy(&content_size, buffer + offset, sizeof(content_size)); offset += sizeof(content_size);

    if (offset + content_size > size) {
        return -1;
    }

    // Проверяем тип и копируем контент
    switch (msg->type) {
        case MSG_TEXT:
            if (content_size != sizeof(TextMessage)) return -1;
            memcpy(&msg->content.text, buffer + offset, content_size);
            break;
        case MSG_IMAGE:
            if (content_size != sizeof(ImageMessage)) return -1;
            memcpy(&msg->content.image, buffer + offset, content_size);
            break;
        case MSG_FILE:
            if (content_size != sizeof(FileMessage)) return -1;
            memcpy(&msg->content.file, buffer + offset, content_size);
            break;
        case MSG_LOCATION:
            if (content_size != sizeof(LocationMessage)) return -1;
            memcpy(&msg->content.location, buffer + offset, content_size);
            break;
        case MSG_AUDIO:
            if (content_size != sizeof(AudioMessage)) return -1;
            memcpy(&msg->content.audio, buffer + offset, content_size);
            break;
        default:
            return -1;
    }

    return 0;
}

// Вывод сообщения 

const char* get_type_name(MessageType type) {
    switch (type) {
        case MSG_TEXT: return "Текст";
        case MSG_IMAGE: return "Изображение";
        case MSG_FILE: return "Файл";
        case MSG_LOCATION: return "Локация";
        case MSG_AUDIO: return "Аудио";
        default: return "Неизвестно";
    }
}

void message_print(const Message *msg) {
    if (!msg) return;

    struct tm *tm_info = localtime(&msg->timestamp);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M", tm_info);

    printf("[%s] User#%u: ", time_str, msg->sender_id);

    switch (msg->type) {
        case MSG_TEXT:
            printf("%s%s", msg->content.text.content,
                   msg->content.text.is_edited ? " (изменено)" : "");
            break;
        case MSG_IMAGE:
            printf("[Изображение: %s (%dx%d, %.1f MB)]",
                   msg->content.image.filename,
                   msg->content.image.width,
                   msg->content.image.height,
                   msg->content.image.size_bytes / (1024.0 * 1024.0));
            break;
        case MSG_FILE:
            printf("[Файл: %s (%s, %.1f KB)]",
                   msg->content.file.filename,
                   msg->content.file.mime_type,
                   msg->content.file.size_bytes / 1024.0);
            break;
        case MSG_LOCATION:
            printf("[Локация: %.4f, %.4f - %s]",
                   msg->content.location.latitude,
                   msg->content.location.longitude,
                   msg->content.location.address);
            break;
        case MSG_AUDIO:
            printf("[Аудио: %s (%d:%02d, %.1f KB)]",
                   msg->content.audio.filename,
                   msg->content.audio.duration_seconds / 60,
                   msg->content.audio.duration_seconds % 60,
                   msg->content.audio.size_bytes / 1024.0);
            break;
    }
    printf("\n");
}

//Фильтрация сообщений 

size_t filter_messages_by_type(const Message *messages, size_t count,
                               MessageType type, Message *result) {
    size_t found = 0;
    for (size_t i = 0; i < count; i++) {
        if (messages[i].type == type) {
            result[found++] = messages[i];
        }
    }
    return found;
}

size_t filter_messages_by_sender(const Message *messages, size_t count,
                                 unsigned int sender_id, Message *result) {
    size_t found = 0;
    for (size_t i = 0; i < count; i++) {
        if (messages[i].sender_id == sender_id) {
            result[found++] = messages[i];
        }
    }
    return found;
}

//Демонстрация 

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand((unsigned int)time(NULL));

    printf("=== Система сообщений (Protocol Buffers style) ===\n\n");

    // Создаём сообщения разных типов
    Message msg1 = create_text_message(1, 42, "Привет! Как дела?");
    Message msg2 = create_text_message(2, 42, "Отлично! Смотри что нашёл:");
    Message msg3 = create_image_message(2, 42, "photo.jpg", 1920, 1080, 2621440);
    Message msg4 = create_text_message(1, 42, "Круто! Где это?");
    Message msg5 = create_location_message(2, 42, 55.7558, 37.6173, "Москва, Красная площадь");
    Message msg6 = create_audio_message(1, 42, "voice_message.ogg", 45, 72000);
    Message msg7 = create_file_message(2, 42, "document.pdf", "application/pdf", 524288);

    // Массив всех сообщений
    Message messages[] = {msg1, msg2, msg3, msg4, msg5, msg6, msg7};
    size_t msg_count = sizeof(messages) / sizeof(messages[0]);

    // Вывод чата
    printf("=== Чат #42 ===\n");
    for (size_t i = 0; i < msg_count; i++) {
        message_print(&messages[i]);
    }

    // Фильтрация по типу
    printf("\n=== Только изображения ===\n");
    Message images[10];
    size_t img_count = filter_messages_by_type(messages, msg_count, MSG_IMAGE, images);
    for (size_t i = 0; i < img_count; i++) {
        message_print(&images[i]);
    }

    printf("\n=== Только локации ===\n");
    Message locations[10];
    size_t loc_count = filter_messages_by_type(messages, msg_count, MSG_LOCATION, locations);
    for (size_t i = 0; i < loc_count; i++) {
        message_print(&locations[i]);
    }

    // Фильтрация по отправителю
    printf("\n=== Сообщения от User#1 ===\n");
    Message from_user1[10];
    size_t user1_count = filter_messages_by_sender(messages, msg_count, 1, from_user1);
    for (size_t i = 0; i < user1_count; i++) {
        message_print(&from_user1[i]);
    }

    // Сериализация/десериализация
    printf("\n=== Проверка сериализации ===\n");
    unsigned char buffer[1024];
    size_t serialized_size = message_serialize(&msg5, buffer, sizeof(buffer));
    printf("Размер сериализованного сообщения: %zu байт\n", serialized_size);

    Message deserialized;
    if (message_deserialize(buffer, serialized_size, &deserialized) == 0) {
        printf("Десериализованное сообщение: ");
        message_print(&deserialized);
    } else {
        printf("Ошибка десериализации!\n");
    }

    return 0;
}
