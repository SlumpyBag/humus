/*
 * Humus Core Implementation
 * Main implementation file for core functionality
 */

#include "humus_core.h"

#ifdef HUMUS_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
#elif defined(HUMUS_PLATFORM_LINUX) || defined(HUMUS_PLATFORM_MACOS)
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

/* ============ Core Functions ============ */

int hm_init(void) {
    printf("Humus v%s initialized\n", HUMUS_VERSION);
#ifdef HUMUS_PLATFORM_WINDOWS
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return -1;
    }
#endif
    return 0;
}

void hm_shutdown(void) {
    printf("Humus shutting down...\n");
#ifdef HUMUS_PLATFORM_WINDOWS
    WSACleanup();
#endif
}

/* ============ Utility Functions ============ */

hm_float64 hm_lerp(hm_float64 a, hm_float64 b, hm_float64 t) {
    return a + (b - a) * t;
}

hm_color hm_color_new(hm_uint8 r, hm_uint8 g, hm_uint8 b, hm_uint8 a) {
    hm_color c = {r, g, b, a};
    return c;
}

hm_color hm_color_lerp(hm_color a, hm_color b, hm_float64 t) {
    hm_color result;
    result.r = (hm_uint8)hm_lerp(a.r, b.r, t);
    result.g = (hm_uint8)hm_lerp(a.g, b.g, t);
    result.b = (hm_uint8)hm_lerp(a.b, b.b, t);
    result.a = (hm_uint8)hm_lerp(a.a, b.a, t);
    return result;
}

/* ============ Window Functions (Stub implementations) ============ */

hm_window* hm_window_create(int width, int height, const char* title) {
    hm_window* win = (hm_window*)malloc(sizeof(hm_window));
    if (!win) return NULL;
    
    win->width = width;
    win->height = height;
    win->title = strdup(title);
    win->is_open = HM_TRUE;
    win->handle = NULL;
    
    printf("Window created: %dx%d - %s\n", width, height, title);
    return win;
}

void hm_window_destroy(hm_window* win) {
    if (win) {
        free(win->title);
        free(win);
    }
}

hm_bool hm_window_should_close(hm_window* win) {
    return !win->is_open;
}

void hm_window_swap_buffers(hm_window* win) {
    /* Platform-specific buffer swap would go here */
}

void hm_window_poll_events(hm_window* win) {
    /* Platform-specific event polling would go here */
}

/* ============ Button Functions ============ */

hm_button* hm_button_create(int x, int y, int width, int height, const char* label) {
    hm_button* btn = (hm_button*)malloc(sizeof(hm_button));
    if (!btn) return NULL;
    
    btn->x = x;
    btn->y = y;
    btn->width = width;
    btn->height = height;
    btn->label = strdup(label);
    btn->on_click = NULL;
    btn->user_data = NULL;
    btn->is_pressed = HM_FALSE;
    
    printf("Button created: %s at (%d, %d)\n", label, x, y);
    return btn;
}

void hm_button_destroy(hm_button* btn) {
    if (btn) {
        free(btn->label);
        free(btn);
    }
}

void hm_button_set_callback(hm_button* btn, void (*callback)(void*), void* user_data) {
    if (btn) {
        btn->on_click = callback;
        btn->user_data = user_data;
    }
}

hm_bool hm_button_is_clicked(hm_button* btn) {
    /* Real implementation would check mouse state */
    return btn->is_pressed;
}

/* ============ Graphing Functions ============ */

hm_graph* hm_graph_create(const char* title) {
    hm_graph* graph = (hm_graph*)malloc(sizeof(hm_graph));
    if (!graph) return NULL;
    
    graph->title = strdup(title);
    graph->capacity = 1000;
    graph->count = 0;
    graph->points = (hm_graph_point*)malloc(sizeof(hm_graph_point) * graph->capacity);
    graph->line_color = hm_color_new(0, 100, 200, 255);
    
    return graph;
}

void hm_graph_destroy(hm_graph* graph) {
    if (graph) {
        free(graph->title);
        free(graph->points);
        free(graph);
    }
}

void hm_graph_add_point(hm_graph* graph, hm_float64 x, hm_float64 y) {
    if (graph->count >= graph->capacity) {
        graph->capacity *= 2;
        graph->points = (hm_graph_point*)realloc(graph->points, 
                          sizeof(hm_graph_point) * graph->capacity);
    }
    graph->points[graph->count].x = x;
    graph->points[graph->count].y = y;
    graph->count++;
}

void hm_graph_plot_function(hm_graph* graph, hm_float64 (*func)(hm_float64),
                            hm_float64 start, hm_float64 end, int samples) {
    for (int i = 0; i < samples; i++) {
        hm_float64 x = start + (end - start) * i / (samples - 1);
        hm_graph_add_point(graph, x, func(x));
    }
}

void hm_graph_render(hm_graph* graph, hm_window* win) {
    printf("Rendering graph: %s with %d points\n", graph->title, graph->count);
    /* Actual rendering would happen here */
}

/* ============ 3D Calculation Functions ============ */

hm_vec3 hm_3d_rotate_x(hm_vec3 v, hm_float64 angle) {
    hm_float64 c = cos(angle);
    hm_float64 s = sin(angle);
    hm_vec3 result = {
        v.x,
        v.y * c - v.z * s,
        v.y * s + v.z * c
    };
    return result;
}

hm_vec3 hm_3d_rotate_y(hm_vec3 v, hm_float64 angle) {
    hm_float64 c = cos(angle);
    hm_float64 s = sin(angle);
    hm_vec3 result = {
        v.x * c + v.z * s,
        v.y,
        -v.x * s + v.z * c
    };
    return result;
}

hm_vec3 hm_3d_rotate_z(hm_vec3 v, hm_float64 angle) {
    hm_float64 c = cos(angle);
    hm_float64 s = sin(angle);
    hm_vec3 result = {
        v.x * c - v.y * s,
        v.x * s + v.y * c,
        v.z
    };
    return result;
}

hm_vec3 hm_3d_project(hm_vec3 v, int width, int height, hm_float64 fov) {
    hm_float64 scale = fov / (v.z > 0 ? v.z : 1);
    hm_vec3 result = {
        (v.x * scale + 1) * width / 2,
        (1 - v.y * scale) * height / 2,
        v.z
    };
    return result;
}

/* ============ Ray Tracing Functions ============ */

hm_ray hm_ray_create(hm_vec3 origin, hm_vec3 direction) {
    hm_ray ray = {origin, hm_vec3_normalize(direction)};
    return ray;
}

hm_sphere* hm_sphere_create(hm_vec3 center, hm_float64 radius, hm_color color) {
    hm_sphere* sphere = (hm_sphere*)malloc(sizeof(hm_sphere));
    sphere->center = center;
    sphere->radius = radius;
    sphere->color = color;
    return sphere;
}

hm_world* hm_world_create(void) {
    hm_world* world = (hm_world*)malloc(sizeof(hm_world));
    world->capacity = 100;
    world->count = 0;
    world->spheres = (hm_sphere**)malloc(sizeof(hm_sphere*) * world->capacity);
    return world;
}

void hm_world_destroy(hm_world* world) {
    if (world) {
        for (int i = 0; i < world->count; i++) {
            free(world->spheres[i]);
        }
        free(world->spheres);
        free(world);
    }
}

void hm_world_add_sphere(hm_world* world, hm_sphere* sphere) {
    if (world->count >= world->capacity) {
        world->capacity *= 2;
        world->spheres = (hm_sphere**)realloc(world->spheres,
                        sizeof(hm_sphere*) * world->capacity);
    }
    world->spheres[world->count++] = sphere;
}

hm_bool hm_ray_hit_sphere(hm_ray ray, hm_sphere* sphere, hm_hit_record* record) {
    hm_vec3 oc = hm_vec3_sub(ray.origin, sphere->center);
    hm_float64 b = hm_vec3_dot(oc, ray.direction);
    hm_float64 c = hm_vec3_dot(oc, oc) - sphere->radius * sphere->radius;
    hm_float64 discriminant = b * b - c;
    
    if (discriminant < 0) return HM_FALSE;
    
    hm_float64 t = -b - sqrt(discriminant);
    if (t < 0.0001) {
        t = -b + sqrt(discriminant);
        if (t < 0.0001) return HM_FALSE;
    }
    
    record->t = t;
    record->point = hm_vec3_add(ray.origin, hm_vec3_scale(ray.direction, t));
    record->normal = hm_vec3_normalize(hm_vec3_sub(record->point, sphere->center));
    record->sphere = sphere;
    
    return HM_TRUE;
}

hm_bool hm_world_hit(hm_world* world, hm_ray ray, hm_hit_record* record) {
    hm_hit_record temp;
    hm_bool hit_anything = HM_FALSE;
    hm_float64 closest_so_far = 1000000.0;
    
    for (int i = 0; i < world->count; i++) {
        if (hm_ray_hit_sphere(ray, world->spheres[i], &temp)) {
            if (temp.t < closest_so_far) {
                closest_so_far = temp.t;
                *record = temp;
                hit_anything = HM_TRUE;
            }
        }
    }
    
    return hit_anything;
}

hm_color hm_trace_ray(hm_world* world, hm_ray ray, int depth) {
    hm_hit_record record;
    
    if (depth <= 0) {
        return hm_color_new(0, 0, 0, 255);
    }
    
    if (hm_world_hit(world, ray, &record)) {
        /* Simple diffuse shading based on normal */
        hm_vec3 light_dir = hm_vec3_normalize((hm_vec3){1, 1, 1});
        hm_float64 intensity = fmax(0.0, hm_vec3_dot(record.normal, light_dir));
        
        return hm_color_new(
            (hm_uint8)(record.sphere->color.r * intensity),
            (hm_uint8)(record.sphere->color.g * intensity),
            (hm_uint8)(record.sphere->color.b * intensity),
            255
        );
    }
    
    /* Sky gradient */
    hm_float64 t = 0.5 * (ray.direction.y + 1.0);
    return hm_color_lerp(
        hm_color_new(255, 255, 255, 255),
        hm_color_new(135, 206, 235, 255),
        t
    );
}

void hm_render_scene(hm_world* world, hm_vec3 camera_pos, int width, int height, hm_color* pixels) {
    hm_vec3 lower_left_corner = {-2, -1, -1};
    hm_vec3 horizontal = {4, 0, 0};
    hm_vec3 vertical = {0, 2, 0};
    
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            hm_float64 u = (hm_float64)i / width;
            hm_float64 v = (hm_float64)j / height;
            
            hm_vec3 direction = hm_vec3_sub(
                hm_vec3_add(
                    hm_vec3_add(lower_left_corner, hm_vec3_scale(horizontal, u)),
                    hm_vec3_scale(vertical, v)
                ),
                camera_pos
            );
            
            hm_ray ray = hm_ray_create(camera_pos, direction);
            pixels[j * width + i] = hm_trace_ray(world, ray, 10);
        }
    }
}

/* ============ Web Interaction Functions ============ */

hm_http_response* hm_http_get(const char* url) {
    printf("HTTP GET: %s\n", url);
    
    hm_http_response* resp = (hm_http_response*)malloc(sizeof(hm_http_response));
    resp->status_code = 200;
    resp->headers = strdup("Content-Type: text/plain");
    resp->body = strdup("Web interaction stub - implement platform-specific HTTP");
    
    return resp;
}

hm_http_response* hm_http_post(const char* url, const char* data) {
    printf("HTTP POST: %s with data: %s\n", url, data);
    
    hm_http_response* resp = (hm_http_response*)malloc(sizeof(hm_http_response));
    resp->status_code = 200;
    resp->headers = strdup("Content-Type: text/plain");
    resp->body = strdup("POST successful (stub)");
    
    return resp;
}

void hm_http_response_destroy(hm_http_response* resp) {
    if (resp) {
        free(resp->headers);
        free(resp->body);
        free(resp);
    }
}

/* ============ P2P Chat Functions ============ */

hm_chatroom* hm_chatroom_create(void) {
    hm_chatroom* room = (hm_chatroom*)calloc(1, sizeof(hm_chatroom));
    room->peer_count = 0;
    room->message_count = 0;
    room->socket_fd = -1;
    return room;
}

void hm_chatroom_destroy(hm_chatroom* room) {
    if (room) {
        free(room);
    }
}

hm_bool hm_chatroom_connect(hm_chatroom* room, const char* address, int port) {
    printf("P2P connecting to %s:%d\n", address, port);
    /* Actual socket connection would go here */
    room->socket_fd = 0; /* Stub */
    return HM_TRUE;
}

hm_bool hm_chatroom_send(hm_chatroom* room, const char* message) {
    printf("P2P sending: %s\n", message);
    /* Actual send would go here */
    return HM_TRUE;
}

hm_chat_message* hm_chatroom_receive(hm_chatroom* room) {
    /* Actual receive would go here */
    return NULL;
}

/* ============ Vector Image Functions ============ */

hm_vector_image* hm_vector_image_create(int width, int height) {
    hm_vector_image* img = (hm_vector_image*)malloc(sizeof(hm_vector_image));
    img->width = width;
    img->height = height;
    img->capacity = 100;
    img->count = 0;
    img->shapes = (hm_shape*)malloc(sizeof(hm_shape) * img->capacity);
    return img;
}

void hm_vector_image_destroy(hm_vector_image* img) {
    if (img) {
        free(img->shapes);
        free(img);
    }
}

void hm_vector_image_add_line(hm_vector_image* img, hm_vec2 start, hm_vec2 end,
                              hm_color stroke, hm_float64 stroke_width) {
    if (img->count >= img->capacity) {
        img->capacity *= 2;
        img->shapes = (hm_shape*)realloc(img->shapes, sizeof(hm_shape) * img->capacity);
    }
    
    hm_shape* shape = &img->shapes[img->count++];
    shape->type = HM_SHAPE_LINE;
    shape->start = start;
    shape->end = end;
    shape->stroke = stroke;
    shape->stroke_width = stroke_width;
}

void hm_vector_image_add_rect(hm_vector_image* img, hm_vec2 pos, hm_vec2 size,
                              hm_color stroke, hm_color fill, hm_float64 stroke_width) {
    if (img->count >= img->capacity) {
        img->capacity *= 2;
        img->shapes = (hm_shape*)realloc(img->shapes, sizeof(hm_shape) * img->capacity);
    }
    
    hm_shape* shape = &img->shapes[img->count++];
    shape->type = HM_SHAPE_RECTANGLE;
    shape->start = pos;
    shape->end = (hm_vec2){pos.x + size.x, pos.y + size.y};
    shape->stroke = stroke;
    shape->fill = fill;
    shape->stroke_width = stroke_width;
}

void hm_vector_image_add_circle(hm_vector_image* img, hm_vec2 center, hm_float64 radius,
                                hm_color stroke, hm_color fill, hm_float64 stroke_width) {
    if (img->count >= img->capacity) {
        img->capacity *= 2;
        img->shapes = (hm_shape*)realloc(img->shapes, sizeof(hm_shape) * img->capacity);
    }
    
    hm_shape* shape = &img->shapes[img->count++];
    shape->type = HM_SHAPE_CIRCLE;
    shape->start = center;
    shape->radius = radius;
    shape->stroke = stroke;
    shape->fill = fill;
    shape->stroke_width = stroke_width;
}

void hm_vector_image_render(hm_vector_image* img, hm_window* win) {
    printf("Rendering vector image: %dx%d with %d shapes\n", img->width, img->height, img->count);
}

hm_bool hm_vector_image_save_svg(hm_vector_image* img, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return HM_FALSE;
    
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n",
            img->width, img->height);
    
    for (int i = 0; i < img->count; i++) {
        hm_shape* s = &img->shapes[i];
        
        if (s->type == HM_SHAPE_LINE) {
            fprintf(f, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ",
                    s->start.x, s->start.y, s->end.x, s->end.y);
            fprintf(f, "stroke=\"rgb(%d,%d,%d)\" stroke-width=\"%.2f\"/>\n",
                    s->stroke.r, s->stroke.g, s->stroke.b, s->stroke_width);
        } else if (s->type == HM_SHAPE_RECTANGLE) {
            fprintf(f, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" ",
                    s->start.x, s->start.y, s->end.x - s->start.x, s->end.y - s->start.y);
            fprintf(f, "fill=\"rgb(%d,%d,%d)\" stroke=\"rgb(%d,%d,%d)\" stroke-width=\"%.2f\"/>\n",
                    s->fill.r, s->fill.g, s->fill.b, s->stroke.r, s->stroke.g, s->stroke.b, s->stroke_width);
        } else if (s->type == HM_SHAPE_CIRCLE) {
            fprintf(f, "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" ",
                    s->start.x, s->start.y, s->radius);
            fprintf(f, "fill=\"rgb(%d,%d,%d)\" stroke=\"rgb(%d,%d,%d)\" stroke-width=\"%.2f\"/>\n",
                    s->fill.r, s->fill.g, s->fill.b, s->stroke.r, s->stroke.g, s->stroke.b, s->stroke_width);
        }
    }
    
    fprintf(f, "</svg>\n");
    fclose(f);
    
    printf("Vector image saved to: %s\n", filename);
    return HM_TRUE;
}

/* ============ VM / .hes Execution Functions ============ */

hm_vm* hm_vm_create(void) {
    hm_vm* vm = (hm_vm*)calloc(1, sizeof(hm_vm));
    vm->stack_size = 4096;
    vm->heap_size = 65536;
    vm->stack = malloc(vm->stack_size);
    vm->heap = malloc(vm->heap_size);
    return vm;
}

void hm_vm_destroy(hm_vm* vm) {
    if (vm) {
        free(vm->source_code);
        free(vm->stack);
        free(vm->heap);
        free(vm);
    }
}

hm_bool hm_vm_load_file(hm_vm* vm, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        printf("Error: Could not open file: %s\n", filename);
        return HM_FALSE;
    }
    
    fseek(f, 0, SEEK_END);
    vm->source_length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    vm->source_code = (char*)malloc(vm->source_length + 1);
    fread(vm->source_code, 1, vm->source_length, f);
    vm->source_code[vm->source_length] = '\0';
    
    fclose(f);
    
    printf("Loaded .hes file: %s (%d bytes)\n", filename, vm->source_length);
    return HM_TRUE;
}

hm_bool hm_vm_run(hm_vm* vm) {
    printf("Executing .hes bytecode...\n");
    /* Full VM implementation would go here */
    /* This is a stub that just prints the source */
    if (vm->source_code) {
        printf("Source preview: %.100s...\n", vm->source_code);
    }
    return HM_TRUE;
}
