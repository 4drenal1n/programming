#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//  Вариант 1: Битовые флаги через enum 

typedef enum {
    PERM_NONE     = 0,
    PERM_READ     = 1 << 0,  // 00001
    PERM_WRITE    = 1 << 1,  // 00010
    PERM_EXECUTE  = 1 << 2,  // 00100
    PERM_DELETE   = 1 << 3,  // 01000
    PERM_ADMIN    = 1 << 4,  // 10000
    PERM_ALL      = PERM_READ | PERM_WRITE | PERM_EXECUTE | PERM_DELETE | PERM_ADMIN
} Permission;

//  Вариант 2: Битовые поля 

typedef struct {
    unsigned int read     : 1;
    unsigned int write    : 1;
    unsigned int execute  : 1;
    unsigned int delete   : 1;
    unsigned int admin    : 1;
    unsigned int reserved : 27;  // Резерв для выравнивания
} PermissionBits;

//  Пользователь и ресурс 

typedef struct {
    unsigned int id;
    char name[50];
    Permission permissions;
} User;

typedef struct {
    unsigned int id;
    char name[100];
    Permission required_permissions;
} Resource;

// Работа с правами

void permission_add(Permission *p, Permission flag) {
    if (p) *p = (Permission)(*p | flag);
}

void permission_remove(Permission *p, Permission flag) {
    if (p) *p = (Permission)(*p & ~flag);
}

int permission_has(Permission p, Permission flag) {
    return (p & flag) == flag;
}

int permission_has_all(Permission p, Permission flags) {
    return (p & flags) == flags;
}

int permission_has_any(Permission p, Permission flags) {
    return (p & flags) != PERM_NONE;
}

//  Преобразование в строку 

void permission_to_string(Permission p, char *buffer, size_t size) {
    if (!buffer || size < 6) return;

    buffer[0] = (p & PERM_READ)    ? 'r' : '-';
    buffer[1] = (p & PERM_WRITE)   ? 'w' : '-';
    buffer[2] = (p & PERM_EXECUTE) ? 'x' : '-';
    buffer[3] = (p & PERM_DELETE)  ? 'd' : '-';
    buffer[4] = (p & PERM_ADMIN)   ? 'a' : '-';
    buffer[5] = '\0';
}

//  Проверка доступа 

int user_can_access(const User *user, const Resource *resource) {
    if (!user || !resource) return 0;
    // Пользователь имеет доступ, если у него есть ВСЕ требуемые права
    return permission_has_all(user->permissions, resource->required_permissions);
}

//  Вывод 

void user_print(const User *user) {
    if (!user) return;

    char perm_str[10];
    permission_to_string(user->permissions, perm_str, sizeof(perm_str));

    printf("Пользователь: %s (ID: %u)\n", user->name, user->id);
    printf("Права: %s", perm_str);

    // Расшифровка прав
    printf(" (");
    int first = 1;
    if (user->permissions & PERM_READ)    { printf("%sчтение", first ? "" : ", "); first = 0; }
    if (user->permissions & PERM_WRITE)   { printf("%sзапись", first ? "" : ", "); first = 0; }
    if (user->permissions & PERM_EXECUTE) { printf("%sвыполнение", first ? "" : ", "); first = 0; }
    if (user->permissions & PERM_DELETE)  { printf("%sудаление", first ? "" : ", "); first = 0; }
    if (user->permissions & PERM_ADMIN)   { printf("%sадмин", first ? "" : ", "); first = 0; }
    if (user->permissions == PERM_NONE)   { printf("нет прав"); }
    printf(")\n\n");
}

void resource_print(const Resource *resource) {
    if (!resource) return;

    char perm_str[10];
    permission_to_string(resource->required_permissions, perm_str, sizeof(perm_str));

    printf("Ресурс: %s (ID: %u)\n", resource->name, resource->id);
    printf("Требуемые права: %s\n\n", perm_str);
}

//  Работа с битовыми полями 

void permission_bits_init(PermissionBits *pb) {
    if (!pb) return;
    pb->read = pb->write = pb->execute = pb->delete = pb->admin = 0;
    pb->reserved = 0;
}

void permission_bits_set(PermissionBits *pb, Permission p) {
    if (!pb) return;
    pb->read    = (p & PERM_READ)    ? 1 : 0;
    pb->write   = (p & PERM_WRITE)   ? 1 : 0;
    pb->execute = (p & PERM_EXECUTE) ? 1 : 0;
    pb->delete  = (p & PERM_DELETE)  ? 1 : 0;
    pb->admin   = (p & PERM_ADMIN)   ? 1 : 0;
}

Permission permission_bits_to_enum(const PermissionBits *pb) {
    Permission p = PERM_NONE;
    if (!pb) return p;

    if (pb->read)    p |= PERM_READ;
    if (pb->write)   p |= PERM_WRITE;
    if (pb->execute) p |= PERM_EXECUTE;
    if (pb->delete)  p |= PERM_DELETE;
    if (pb->admin)   p |= PERM_ADMIN;
    return p;
}

//  Демонстрация 

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    printf("=== Система прав доступа с битовыми флагами ===\n\n");

    // Создаём пользователей
    User admin = {1, "admin", PERM_ALL};
    User editor = {2, "editor", (Permission)(PERM_READ | PERM_WRITE | PERM_EXECUTE)};
    User viewer = {3, "viewer", PERM_READ};
    User guest = {4, "guest", PERM_NONE};

    printf("--- Пользователи ---\n");
    user_print(&admin);
    user_print(&editor);
    user_print(&viewer);
    user_print(&guest);

    // Создаём ресурсы
    Resource public_file = {1, "/data/public.txt", PERM_READ};
    Resource edit_file = {2, "/data/edit.txt", (Permission)(PERM_READ | PERM_WRITE)};
    Resource secret_file = {3, "/data/secret.txt", (Permission)(PERM_READ | PERM_WRITE | PERM_ADMIN)};
    Resource exec_file = {4, "/bin/app.exe", (Permission)(PERM_READ | PERM_EXECUTE)};

    printf("--- Ресурсы ---\n");
    resource_print(&public_file);
    resource_print(&edit_file);
    resource_print(&secret_file);
    resource_print(&exec_file);

    // Проверка доступа
    printf("=== Проверка доступа ===\n\n");

    User* users[] = {&admin, &editor, &viewer, &guest};
    Resource* resources[] = {&public_file, &edit_file, &secret_file, &exec_file};
    const char* user_names[] = {"admin", "editor", "viewer", "guest"};

    for (size_t r = 0; r < sizeof(resources)/sizeof(resources[0]); r++) {
        printf("Ресурс: %s\n", resources[r]->name);
        char req_perm[10];
        permission_to_string(resources[r]->required_permissions, req_perm, sizeof(req_perm));
        printf("Требуемые права: %s\n", req_perm);

        for (size_t u = 0; u < sizeof(users)/sizeof(users[0]); u++) {
            int access = user_can_access(users[u], resources[r]);
            printf("  %s -> %s\n", user_names[u], access ? "разрешён" : "ЗАПРЕЩЁН");
        }
        printf("\n");
    }

    // Демонстрация операций с флагами
    printf("=== Операции с флагами ===\n\n");

    Permission user_perm = PERM_READ;
    char perm_str[10];

    printf("Начальные права: ");
    permission_to_string(user_perm, perm_str, sizeof(perm_str));
    printf("%s\n", perm_str);

    permission_add(&user_perm, PERM_WRITE);
    printf("После добавления WRITE: ");
    permission_to_string(user_perm, perm_str, sizeof(perm_str));
    printf("%s\n", perm_str);

    permission_add(&user_perm, PERM_DELETE);
    printf("После добавления DELETE: ");
    permission_to_string(user_perm, perm_str, sizeof(perm_str));
    printf("%s\n", perm_str);

    printf("Проверка наличия WRITE: %s\n", permission_has(user_perm, PERM_WRITE) ? "да" : "нет");
    printf("Проверка наличия ADMIN: %s\n", permission_has(user_perm, PERM_ADMIN) ? "да" : "нет");
    printf("Проверка наличия всех (READ|WRITE): %s\n",
           permission_has_all(user_perm, (Permission)(PERM_READ | PERM_WRITE)) ? "да" : "нет");
    printf("Проверка наличия любого из (DELETE|ADMIN): %s\n",
           permission_has_any(user_perm, (Permission)(PERM_DELETE | PERM_ADMIN)) ? "да" : "нет");

    permission_remove(&user_perm, PERM_DELETE);
    printf("После удаления DELETE: ");
    permission_to_string(user_perm, perm_str, sizeof(perm_str));
    printf("%s\n", perm_str);

    // Демонстрация битовых полей
    printf("\n=== Битовые поля ===\n\n");

    PermissionBits pb;
    permission_bits_init(&pb);
    permission_bits_set(&pb, (Permission)(PERM_READ | PERM_WRITE | PERM_EXECUTE));

    printf("Битовые поля: r=%d w=%d x=%d d=%d a=%d\n",
           pb.read, pb.write, pb.execute, pb.delete, pb.admin);

    Permission converted = permission_bits_to_enum(&pb);
    permission_to_string(converted, perm_str, sizeof(perm_str));
    printf("Преобразование в enum: %s\n", perm_str);

    printf("Размер Permission (enum): %zu байт\n", sizeof(Permission));
    printf("Размер PermissionBits (struct): %zu байт\n", sizeof(PermissionBits));

    return 0;
}
