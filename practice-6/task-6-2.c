#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <windows.h>


// Типы фигур 
typedef enum {
    SHAPE_POINT,
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE,
    SHAPE_POLYGON
} ShapeType;

// Точка
typedef struct {
    double x, y;
} Point;

// Круг
typedef struct {
    Point center;
    double radius;
} Circle;

// Прямоугольник
typedef struct {
    Point top_left;
    double width, height;
} RectShape;

// Треугольник
typedef struct {
    Point a, b, c;
} Triangle;

// Многоугольник
typedef struct {
    Point *vertices;  // динамический массив
    int count;
} PolyShape;

// Размеченное объединение (tagged union)
typedef struct {
    ShapeType type;
    union {
        Point point;
        Circle circle;
        RectShape rectangle;
        Triangle triangle;
        PolyShape polygon;
    } data;
} Shape;

// Конструкторы 
Shape shape_point(double x, double y) {
    Shape s;
    s.type = SHAPE_POINT;
    s.data.point.x = x;
    s.data.point.y = y;
    return s;
}

Shape shape_circle(double cx, double cy, double radius) {
    Shape s;
    s.type = SHAPE_CIRCLE;
    s.data.circle.center.x = cx;
    s.data.circle.center.y = cy;
    s.data.circle.radius = radius;
    return s;
}

Shape shape_rectangle(double x, double y, double w, double h) {
    Shape s;
    s.type = SHAPE_RECTANGLE;
    s.data.rectangle.top_left.x = x;
    s.data.rectangle.top_left.y = y;
    s.data.rectangle.width = w;
    s.data.rectangle.height = h;
    return s;
}

Shape shape_triangle(Point a, Point b, Point c) {
    Shape s;
    s.type = SHAPE_TRIANGLE;
    s.data.triangle.a = a;
    s.data.triangle.b = b;
    s.data.triangle.c = c;
    return s;
}

Shape shape_polygon(Point *verts, int count) {
    Shape s;
    s.type = SHAPE_POLYGON;
    s.data.polygon.count = count;
    s.data.polygon.vertices = (Point*)malloc(count * sizeof(Point));
    for (int i = 0; i < count; i++)
        s.data.polygon.vertices[i] = verts[i];
    return s;
}

// Освобождение памяти для полигона
void shape_free(Shape *s) {
    if (s->type == SHAPE_POLYGON) {
        free(s->data.polygon.vertices);
        s->data.polygon.vertices = NULL;
        s->data.polygon.count = 0;
    }
}


static double point_distance(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

static double triangle_area(Point a, Point b, Point c) {
    return fabs((a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y)) / 2.0);
}

static double triangle_perimeter(Point a, Point b, Point c) {
    return point_distance(a, b) + point_distance(b, c) + point_distance(c, a);
}

static Point triangle_center(Point a, Point b, Point c) {
    Point center = {(a.x + b.x + c.x)/3.0, (a.y + b.y + c.y)/3.0};
    return center;
}

static double polygon_area(Point *verts, int n) {
    double area = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += verts[i].x * verts[j].y;
        area -= verts[j].x * verts[i].y;
    }
    return fabs(area) / 2.0;
}

static double polygon_perimeter(Point *verts, int n) {
    double perim = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        perim += point_distance(verts[i], verts[j]);
    }
    return perim;
}

static Point polygon_center(Point *verts, int n) {
    Point c = {0.0, 0.0};
    for (int i = 0; i < n; i++) {
        c.x += verts[i].x;
        c.y += verts[i].y;
    }
    c.x /= n;
    c.y /= n;
    return c;
}

//Основные функции фигур 
double shape_area(const Shape *s) {
    switch (s->type) {
        case SHAPE_POINT:      return 0.0;
        case SHAPE_CIRCLE:     return M_PI * s->data.circle.radius * s->data.circle.radius;
        case SHAPE_RECTANGLE:  return s->data.rectangle.width * s->data.rectangle.height;
        case SHAPE_TRIANGLE:   return triangle_area(s->data.triangle.a, s->data.triangle.b, s->data.triangle.c);
        case SHAPE_POLYGON:    return polygon_area(s->data.polygon.vertices, s->data.polygon.count);
        default:               return 0.0;
    }
}

double shape_perimeter(const Shape *s) {
    switch (s->type) {
        case SHAPE_POINT:      return 0.0;
        case SHAPE_CIRCLE:     return 2.0 * M_PI * s->data.circle.radius;
        case SHAPE_RECTANGLE:  return 2.0 * (s->data.rectangle.width + s->data.rectangle.height);
        case SHAPE_TRIANGLE:   return triangle_perimeter(s->data.triangle.a, s->data.triangle.b, s->data.triangle.c);
        case SHAPE_POLYGON:    return polygon_perimeter(s->data.polygon.vertices, s->data.polygon.count);
        default:               return 0.0;
    }
}

Point shape_center(const Shape *s) {
    Point c = {0.0, 0.0};
    switch (s->type) {
        case SHAPE_POINT:
            c = s->data.point;
            break;
        case SHAPE_CIRCLE:
            c = s->data.circle.center;
            break;
        case SHAPE_RECTANGLE: {
            RectShape r = s->data.rectangle;
            c.x = r.top_left.x + r.width/2.0;
            c.y = r.top_left.y - r.height/2.0;
            break;
        }
        case SHAPE_TRIANGLE:
            c = triangle_center(s->data.triangle.a, s->data.triangle.b, s->data.triangle.c);
            break;
        case SHAPE_POLYGON:
            c = polygon_center(s->data.polygon.vertices, s->data.polygon.count);
            break;
    }
    return c;
}

// Перемещение
void shape_move(Shape *s, double dx, double dy) {
    switch (s->type) {
        case SHAPE_POINT:
            s->data.point.x += dx;
            s->data.point.y += dy;
            break;
        case SHAPE_CIRCLE:
            s->data.circle.center.x += dx;
            s->data.circle.center.y += dy;
            break;
        case SHAPE_RECTANGLE:
            s->data.rectangle.top_left.x += dx;
            s->data.rectangle.top_left.y += dy;
            break;
        case SHAPE_TRIANGLE:
            s->data.triangle.a.x += dx; s->data.triangle.a.y += dy;
            s->data.triangle.b.x += dx; s->data.triangle.b.y += dy;
            s->data.triangle.c.x += dx; s->data.triangle.c.y += dy;
            break;
        case SHAPE_POLYGON:
            for (int i = 0; i < s->data.polygon.count; i++) {
                s->data.polygon.vertices[i].x += dx;
                s->data.polygon.vertices[i].y += dy;
            }
            break;
    }
}

// Масштабирование относительно центра
void shape_scale(Shape *s, double factor) {
    Point c = shape_center(s);
    switch (s->type) {
        case SHAPE_POINT:
            
            break;
        case SHAPE_CIRCLE:
            s->data.circle.radius *= factor;
            break;
        case SHAPE_RECTANGLE:
            s->data.rectangle.width *= factor;
            s->data.rectangle.height *= factor;
            
            s->data.rectangle.top_left.x = c.x - s->data.rectangle.width/2.0;
            s->data.rectangle.top_left.y = c.y + s->data.rectangle.height/2.0;
            break;
        case SHAPE_TRIANGLE:
            s->data.triangle.a.x = c.x + (s->data.triangle.a.x - c.x) * factor;
            s->data.triangle.a.y = c.y + (s->data.triangle.a.y - c.y) * factor;
            s->data.triangle.b.x = c.x + (s->data.triangle.b.x - c.x) * factor;
            s->data.triangle.b.y = c.y + (s->data.triangle.b.y - c.y) * factor;
            s->data.triangle.c.x = c.x + (s->data.triangle.c.x - c.x) * factor;
            s->data.triangle.c.y = c.y + (s->data.triangle.c.y - c.y) * factor;
            break;
        case SHAPE_POLYGON:
            for (int i = 0; i < s->data.polygon.count; i++) {
                s->data.polygon.vertices[i].x = c.x + (s->data.polygon.vertices[i].x - c.x) * factor;
                s->data.polygon.vertices[i].y = c.y + (s->data.polygon.vertices[i].y - c.y) * factor;
            }
            break;
    }
}

// Проверка, содержит ли фигура точку
int shape_contains_point(const Shape *s, Point p) {
    switch (s->type) {
        case SHAPE_POINT:
            return (p.x == s->data.point.x && p.y == s->data.point.y);
        case SHAPE_CIRCLE: {
            Circle c = s->data.circle;
            double dx = p.x - c.center.x;
            double dy = p.y - c.center.y;
            return (dx*dx + dy*dy <= c.radius * c.radius);
        }
        case SHAPE_RECTANGLE: {
            RectShape r = s->data.rectangle;
            double left = r.top_left.x;
            double right = r.top_left.x + r.width;
            double top = r.top_left.y;
            double bottom = r.top_left.y - r.height;
            double ymin = (top < bottom) ? top : bottom;
            double ymax = (top > bottom) ? top : bottom;
            return (p.x >= left && p.x <= right && p.y >= ymin && p.y <= ymax);
        }
        case SHAPE_TRIANGLE: {
            Triangle t = s->data.triangle;
            double area = triangle_area(t.a, t.b, t.c);
            double a1 = triangle_area(p, t.b, t.c);
            double a2 = triangle_area(t.a, p, t.c);
            double a3 = triangle_area(t.a, t.b, p);
            return fabs(area - (a1 + a2 + a3)) < 1e-9;
        }
        case SHAPE_POLYGON: {
           const PolyShape *poly = &s->data.polygon;
            int inside = 0;
            for (int i = 0, j = poly->count-1; i < poly->count; j = i++) {
                Point vi = poly->vertices[i];
                Point vj = poly->vertices[j];
                if (((vi.y > p.y) != (vj.y > p.y)) &&
                    (p.x < (vj.x - vi.x) * (p.y - vi.y) / (vj.y - vi.y) + vi.x))
                    inside = !inside;
            }
            return inside;
        }
        default: return 0;
    }
}

// Проверка пересечения двух фигур 
int shapes_intersect(const Shape *a, const Shape *b) {
    // Если одна из фигур – точка
    if (a->type == SHAPE_POINT) return shape_contains_point(b, a->data.point);
    if (b->type == SHAPE_POINT) return shape_contains_point(a, b->data.point);
    // Круг-круг
    if (a->type == SHAPE_CIRCLE && b->type == SHAPE_CIRCLE) {
      const  Circle *c1 = &a->data.circle;
      const  Circle *c2 = &b->data.circle;
        double dist = point_distance(c1->center, c2->center);
        return dist <= c1->radius + c2->radius;
    }
    // Круг-прямоугольник
    if (a->type == SHAPE_CIRCLE && b->type == SHAPE_RECTANGLE) {
        RectShape r = b->data.rectangle;
        double left = r.top_left.x;
        double right = r.top_left.x + r.width;
        double top = r.top_left.y;
        double bottom = r.top_left.y - r.height;
        double ymin = (top < bottom) ? top : bottom;
        double ymax = (top > bottom) ? top : bottom;
        double cx = a->data.circle.center.x;
        double cy = a->data.circle.center.y;
        double closestX = (cx < left) ? left : (cx > right) ? right : cx;
        double closestY = (cy < ymin) ? ymin : (cy > ymax) ? ymax : cy;
        double dx = cx - closestX;
        double dy = cy - closestY;
        return (dx*dx + dy*dy) <= a->data.circle.radius * a->data.circle.radius;
    }
    if (a->type == SHAPE_RECTANGLE && b->type == SHAPE_CIRCLE)
        return shapes_intersect(b, a);
    
    Point ca = shape_center(a);
    Point cb = shape_center(b);
    return point_distance(ca, cb) < 1e-6; 
}

// Вывод фигуры
void shape_print(const Shape *s) {
    switch (s->type) {
        case SHAPE_POINT:
            printf("Точка (%.2f, %.2f)", s->data.point.x, s->data.point.y);
            break;
        case SHAPE_CIRCLE:
            printf("Круг: центр (%.2f, %.2f), радиус %.2f",
                   s->data.circle.center.x, s->data.circle.center.y, s->data.circle.radius);
            break;
        case SHAPE_RECTANGLE:
            printf("Прямоугольник: левый верхний угол (%.2f, %.2f), ширина %.2f, высота %.2f",
                   s->data.rectangle.top_left.x, s->data.rectangle.top_left.y,
                   s->data.rectangle.width, s->data.rectangle.height);
            break;
        case SHAPE_TRIANGLE:
            printf("Треугольник: (%.2f,%.2f) (%.2f,%.2f) (%.2f,%.2f)",
                   s->data.triangle.a.x, s->data.triangle.a.y,
                   s->data.triangle.b.x, s->data.triangle.b.y,
                   s->data.triangle.c.x, s->data.triangle.c.y);
            break;
        case SHAPE_POLYGON:
            printf("Многоугольник (%d вершин)", s->data.polygon.count);
            break;
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    // Круг
    Shape circle = shape_circle(0.0, 0.0, 5.0);
    printf("=== Круг ===\n");
    shape_print(&circle);
    printf("\nПлощадь: %.2f\n", shape_area(&circle));
    printf("Периметр: %.2f\n\n", shape_perimeter(&circle));

    // Прямоугольник
    Shape rect = shape_rectangle(0.0, 0.0, 4.0, 3.0);
    printf("=== Прямоугольник ===\n");
    shape_print(&rect);
    printf("\nПлощадь: %.2f\n", shape_area(&rect));
    printf("Периметр: %.2f\n\n", shape_perimeter(&rect));

    // Треугольник
    Point a = {0.0, 0.0}, b = {4.0, 0.0}, c = {2.0, 3.0};
    Shape triangle = shape_triangle(a, b, c);
    printf("=== Треугольник ===\n");
    shape_print(&triangle);
    printf("\nПлощадь: %.2f\n", shape_area(&triangle));
    printf("Периметр: %.2f\n\n", shape_perimeter(&triangle));

    // Перемещение и масштабирование
    printf("=== Перемещение круга на (1,1) и масштабирование в 2 раза ===\n");
    shape_move(&circle, 1.0, 1.0);
    shape_scale(&circle, 2.0);
    shape_print(&circle);
    printf("\nПлощадь: %.2f\n", shape_area(&circle));
    printf("Новый центр: (%.2f, %.2f)\n\n", shape_center(&circle).x, shape_center(&circle).y);

    // Проверка принадлежности точки
    Point test = {2.0, 2.0};
    printf("Точка (2,2) внутри круга? %s\n", shape_contains_point(&circle, test) ? "да" : "нет");
    printf("Точка (2,2) внутри прямоугольника? %s\n", shape_contains_point(&rect, test) ? "да" : "нет");

    // Проверка пересечения (круг и прямоугольник)
    printf("Круг и прямоугольник пересекаются? %s\n", shapes_intersect(&circle, &rect) ? "да" : "нет");

    
    shape_free(&circle);
    shape_free(&rect);
    shape_free(&triangle);

    return 0;
}
