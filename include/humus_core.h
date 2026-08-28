/*
 * Humus - A Framework and Programming Language
 * Core Header File
 * 
 * Humus is like Java - .hes files run directly off Humus
 * Can be embedded in web browsers or run standalone
 */

#ifndef HUMUS_CORE_H
#define HUMUS_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* Platform detection */
#if defined(_WIN32) || defined(_WIN64)
    #define HUMUS_PLATFORM_WINDOWS 1
#elif defined(__linux__)
    #define HUMUS_PLATFORM_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
    #define HUMUS_PLATFORM_MACOS 1
#endif

/* Version info */
#define HUMUS_VERSION_MAJOR 0
#define HUMUS_VERSION_MINOR 1
#define HUMUS_VERSION_PATCH 0
#define HUMUS_VERSION "0.1.0"

/* Basic types */
typedef unsigned char hm_bool;
typedef unsigned char hm_byte;
typedef signed char hm_int8;
typedef unsigned char hm_uint8;
typedef short hm_int16;
typedef unsigned short hm_uint16;
typedef int hm_int32;
typedef unsigned int hm_uint32;
typedef long long hm_int64;
typedef unsigned long long hm_uint64;
typedef float hm_float32;
typedef double hm_float64;

#define HM_TRUE 1
#define HM_FALSE 0
#define HM_NULL NULL

/* Vector2D for graphics and calculations */
typedef struct {
    hm_float64 x;
    hm_float64 y;
} hm_vec2;

/* Vector3D for 3D calculations and ray tracing */
typedef struct {
    hm_float64 x;
    hm_float64 y;
    hm_float64 z;
} hm_vec3;

/* Color structure */
typedef struct {
    hm_uint8 r;
    hm_uint8 g;
    hm_uint8 b;
    hm_uint8 a;
} hm_color;

/* Window structure */
typedef struct {
    int width;
    int height;
    char* title;
    void* handle;
    hm_bool is_open;
} hm_window;

/* Button structure */
typedef struct {
    int x;
    int y;
    int width;
    int height;
    char* label;
    void (*on_click)(void*);
    void* user_data;
    hm_bool is_pressed;
} hm_button;

/* Graph point */
typedef struct {
    hm_float64 x;
    hm_float64 y;
} hm_graph_point;

/* Graph data */
typedef struct {
    hm_graph_point* points;
    int count;
    int capacity;
    char* title;
    hm_color line_color;
} hm_graph;

/* Ray for ray tracing */
typedef struct {
    hm_vec3 origin;
    hm_vec3 direction;
} hm_ray;

/* Sphere for ray tracing */
typedef struct {
    hm_vec3 center;
    hm_float64 radius;
    hm_color color;
} hm_sphere;

/* Hit record for ray tracing */
typedef struct {
    hm_float64 t;
    hm_vec3 point;
    hm_vec3 normal;
    hm_sphere* sphere;
} hm_hit_record;

/* World for ray tracing */
typedef struct {
    hm_sphere** spheres;
    int count;
    int capacity;
} hm_world;

/* P2P Chat structures */
typedef struct {
    char* peer_id;
    char* address;
    int port;
    hm_bool is_connected;
} hm_peer;

typedef struct {
    char* sender;
    char* message;
    hm_int64 timestamp;
} hm_chat_message;

typedef struct {
    hm_peer* peers;
    int peer_count;
    hm_chat_message* messages;
    int message_count;
    int socket_fd;
} hm_chatroom;

/* Vector Image structures */
typedef enum {
    HM_SHAPE_LINE,
    HM_SHAPE_RECTANGLE,
    HM_SHAPE_CIRCLE,
    HM_SHAPE_PATH
} hm_shape_type;

typedef struct {
    hm_shape_type type;
    hm_vec2 start;
    hm_vec2 end;
    hm_float64 radius;
    hm_color stroke;
    hm_color fill;
    hm_float64 stroke_width;
} hm_shape;

typedef struct {
    hm_shape* shapes;
    int count;
    int capacity;
    int width;
    int height;
} hm_vector_image;

/* HTTP Request/Response for web interaction */
typedef struct {
    char* url;
    char* method;
    char* headers;
    char* body;
} hm_http_request;

typedef struct {
    int status_code;
    char* headers;
    char* body;
} hm_http_response;

/* VM State for .hes execution */
typedef struct {
    char* source_code;
    int source_length;
    int pc; /* Program counter */
    void* stack;
    int stack_size;
    void* heap;
    int heap_size;
} hm_vm;

/* Function pointers for callbacks */
typedef void (*hm_callback)(void*);

/* ============ Vector Operations ============ */
static inline hm_vec3 hm_vec3_add(hm_vec3 a, hm_vec3 b) {
    hm_vec3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

static inline hm_vec3 hm_vec3_sub(hm_vec3 a, hm_vec3 b) {
    hm_vec3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

static inline hm_vec3 hm_vec3_scale(hm_vec3 v, hm_float64 s) {
    hm_vec3 result = {v.x * s, v.y * s, v.z * s};
    return result;
}

static inline hm_float64 hm_vec3_dot(hm_vec3 a, hm_vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline hm_float64 hm_vec3_length(hm_vec3 v) {
    return sqrt(hm_vec3_dot(v, v));
}

static inline hm_vec3 hm_vec3_normalize(hm_vec3 v) {
    hm_float64 len = hm_vec3_length(v);
    if (len > 0) {
        return hm_vec3_scale(v, 1.0 / len);
    }
    return v;
}

/* ============ Core Initialization ============ */
int hm_init(void);
void hm_shutdown(void);

/* ============ Window Functions ============ */
hm_window* hm_window_create(int width, int height, const char* title);
void hm_window_destroy(hm_window* win);
hm_bool hm_window_should_close(hm_window* win);
void hm_window_swap_buffers(hm_window* win);
void hm_window_poll_events(hm_window* win);

/* ============ Button Functions ============ */
hm_button* hm_button_create(int x, int y, int width, int height, const char* label);
void hm_button_destroy(hm_button* btn);
void hm_button_set_callback(hm_button* btn, void (*callback)(void*), void* user_data);
hm_bool hm_button_is_clicked(hm_button* btn);

/* ============ Graphing Functions ============ */
hm_graph* hm_graph_create(const char* title);
void hm_graph_destroy(hm_graph* graph);
void hm_graph_add_point(hm_graph* graph, hm_float64 x, hm_float64 y);
void hm_graph_plot_function(hm_graph* graph, hm_float64 (*func)(hm_float64), 
                            hm_float64 start, hm_float64 end, int samples);
void hm_graph_render(hm_graph* graph, hm_window* win);

/* ============ 3D Calculation Functions ============ */
hm_vec3 hm_3d_rotate_x(hm_vec3 v, hm_float64 angle);
hm_vec3 hm_3d_rotate_y(hm_vec3 v, hm_float64 angle);
hm_vec3 hm_3d_rotate_z(hm_vec3 v, hm_float64 angle);
hm_vec3 hm_3d_project(hm_vec3 v, int width, int height, hm_float64 fov);

/* ============ Ray Tracing Functions ============ */
hm_ray hm_ray_create(hm_vec3 origin, hm_vec3 direction);
hm_sphere* hm_sphere_create(hm_vec3 center, hm_float64 radius, hm_color color);
hm_world* hm_world_create(void);
void hm_world_add_sphere(hm_world* world, hm_sphere* sphere);
void hm_world_destroy(hm_world* world);
hm_bool hm_ray_hit_sphere(hm_ray ray, hm_sphere* sphere, hm_hit_record* record);
hm_bool hm_world_hit(hm_world* world, hm_ray ray, hm_hit_record* record);
hm_color hm_trace_ray(hm_world* world, hm_ray ray, int depth);
void hm_render_scene(hm_world* world, hm_vec3 camera_pos, int width, int height, hm_color* pixels);

/* ============ Web Interaction Functions ============ */
hm_http_response* hm_http_get(const char* url);
hm_http_response* hm_http_post(const char* url, const char* data);
void hm_http_response_destroy(hm_http_response* resp);

/* ============ P2P Chat Functions ============ */
hm_chatroom* hm_chatroom_create(void);
void hm_chatroom_destroy(hm_chatroom* room);
hm_bool hm_chatroom_connect(hm_chatroom* room, const char* address, int port);
hm_bool hm_chatroom_send(hm_chatroom* room, const char* message);
hm_chat_message* hm_chatroom_receive(hm_chatroom* room);

/* ============ Vector Image Functions ============ */
hm_vector_image* hm_vector_image_create(int width, int height);
void hm_vector_image_destroy(hm_vector_image* img);
void hm_vector_image_add_line(hm_vector_image* img, hm_vec2 start, hm_vec2 end, 
                              hm_color stroke, hm_float64 stroke_width);
void hm_vector_image_add_rect(hm_vector_image* img, hm_vec2 pos, hm_vec2 size,
                              hm_color stroke, hm_color fill, hm_float64 stroke_width);
void hm_vector_image_add_circle(hm_vector_image* img, hm_vec2 center, hm_float64 radius,
                                hm_color stroke, hm_color fill, hm_float64 stroke_width);
void hm_vector_image_render(hm_vector_image* img, hm_window* win);
hm_bool hm_vector_image_save_svg(hm_vector_image* img, const char* filename);

/* ============ VM / .hes Execution Functions ============ */
hm_vm* hm_vm_create(void);
void hm_vm_destroy(hm_vm* vm);
hm_bool hm_vm_load_file(hm_vm* vm, const char* filename);
hm_bool hm_vm_run(hm_vm* vm);

/* ============ Utility Functions ============ */
hm_float64 hm_lerp(hm_float64 a, hm_float64 b, hm_float64 t);
hm_color hm_color_new(hm_uint8 r, hm_uint8 g, hm_uint8 b, hm_uint8 a);
hm_color hm_color_lerp(hm_color a, hm_color b, hm_float64 t);

#endif /* HUMUS_CORE_H */
