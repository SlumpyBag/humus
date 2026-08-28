/*
 * Humus Main Entry Point
 * Standalone executable and file association handler
 */

#include "../include/humus_core.h"
#include <string.h>

void print_usage(const char* program) {
    printf("Humus v%s - A Framework and Programming Language\n", HUMUS_VERSION);
    printf("\nUsage:\n");
    printf("  %s [options] [file.hes]\n", program);
    printf("\nOptions:\n");
    printf("  --help, -h     Show this help message\n");
    printf("  --version, -v  Show version information\n");
    printf("  --run, -r      Run a .hes file\n");
    printf("  --demo         Run demo showcasing all features\n");
    printf("\nExamples:\n");
    printf("  %s program.hes           # Run a .hes script\n", program);
    printf("  %s --demo                # Run feature demos\n", program);
    printf("\nFile Associations:\n");
    printf("  .hes files can be double-clicked to run directly with Humus\n");
}

void run_demo(void) {
    printf("\n========== HUMUS FEATURE DEMO ==========\n\n");
    
    /* Initialize */
    hm_init();
    
    /* Window Demo */
    printf("--- Window Creation ---\n");
    hm_window* win = hm_window_create(800, 600, "Humus Demo Window");
    
    /* Button Demo */
    printf("\n--- Button Creation ---\n");
    hm_button* btn = hm_button_create(100, 100, 200, 50, "Click Me!");
    
    void on_button_click(void* data) {
        printf("Button was clicked!\n");
    }
    hm_button_set_callback(btn, on_button_click, NULL);
    
    /* Graphing Demo */
    printf("\n--- Graphing Calculations ---\n");
    hm_graph* graph = hm_graph_create("Sine Wave");
    
    hm_float64 sine_func(hm_float64 x) {
        return sin(x);
    }
    
    hm_graph_plot_function(graph, sine_func, 0, 2 * 3.14159, 100);
    printf("Plotted sine function with %d points\n", graph->count);
    hm_graph_render(graph, win);
    
    /* 3D Calculations Demo */
    printf("\n--- 3D Calculations ---\n");
    hm_vec3 point = {1.0, 0.0, 0.0};
    printf("Original point: (%.2f, %.2f, %.2f)\n", point.x, point.y, point.z);
    
    hm_vec3 rotated = hm_3d_rotate_y(point, 3.14159 / 4);
    printf("Rotated 45° around Y: (%.2f, %.2f, %.2f)\n", rotated.x, rotated.y, rotated.z);
    
    hm_vec3 projected = hm_3d_project(rotated, 800, 600, 1.0);
    printf("Projected to screen: (%.2f, %.2f)\n", projected.x, projected.y);
    
    /* Ray Tracing Demo */
    printf("\n--- Ray Tracing ---\n");
    hm_world* world = hm_world_create();
    
    hm_sphere* sphere1 = hm_sphere_create(
        (hm_vec3){0, 0, -3}, 
        1.0, 
        hm_color_new(255, 100, 100, 255)
    );
    hm_sphere* sphere2 = hm_sphere_create(
        (hm_vec3){2, 0, -4}, 
        1.0, 
        hm_color_new(100, 255, 100, 255)
    );
    hm_sphere* sphere3 = hm_sphere_create(
        (hm_vec3){-2, 0, -4}, 
        1.0, 
        hm_color_new(100, 100, 255, 255)
    );
    
    hm_world_add_sphere(world, sphere1);
    hm_world_add_sphere(world, sphere2);
    hm_world_add_sphere(world, sphere3);
    
    printf("Created ray tracing scene with 3 spheres\n");
    
    /* Small render test */
    hm_color test_pixels[100];
    hm_render_scene(world, (hm_vec3){0, 0, 0}, 10, 10, test_pixels);
    printf("Rendered 10x10 test image (%d pixels)\n", 10 * 10);
    
    /* Web Interaction Demo */
    printf("\n--- Web Interaction ---\n");
    hm_http_response* resp = hm_http_get("https://api.example.com/data");
    printf("HTTP Response: %d - %s\n", resp->status_code, resp->body);
    hm_http_response_destroy(resp);
    
    /* P2P Chat Demo */
    printf("\n--- P2P Chatroom ---\n");
    hm_chatroom* room = hm_chatroom_create();
    hm_chatroom_connect(room, "127.0.0.1", 8080);
    hm_chatroom_send(room, "Hello from Humus!");
    
    /* Vector Image Demo */
    printf("\n--- Vector Image Support ---\n");
    hm_vector_image* img = hm_vector_image_create(400, 300);
    
    hm_vector_image_add_circle(
        img, 
        (hm_vec2){200, 150}, 
        100, 
        hm_color_new(0, 0, 0, 255), 
        hm_color_new(200, 200, 255, 255), 
        2.0
    );
    
    hm_vector_image_add_rect(
        img,
        (hm_vec2){50, 50},
        (hm_vec2){100, 80},
        hm_color_new(255, 0, 0, 255),
        hm_color_new(255, 255, 0, 128),
        3.0
    );
    
    hm_vector_image_add_line(
        img,
        (hm_vec2){0, 0},
        (hm_vec2){400, 300},
        hm_color_new(0, 255, 0, 255),
        5.0
    );
    
    hm_vector_image_save_svg(img, "demo_output.svg");
    
    /* Cleanup */
    hm_vector_image_destroy(img);
    hm_chatroom_destroy(room);
    hm_world_destroy(world); /* Note: would need destroy function */
    hm_graph_destroy(graph);
    hm_button_destroy(btn);
    hm_window_destroy(win);
    
    hm_shutdown();
    
    printf("\n========== DEMO COMPLETE ==========\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }
    
    /* Check for flags */
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        printf("Humus v%s\n", HUMUS_VERSION);
        return 0;
    }
    
    if (strcmp(argv[1], "--demo") == 0) {
        run_demo();
        return 0;
    }
    
    /* Default: try to run as .hes file */
    const char* filename = argv[1];
    
    /* Check if file exists and has .hes extension */
    const char* ext = strrchr(filename, '.');
    if (!ext || strcmp(ext, ".hes") != 0) {
        printf("Warning: File '%s' does not have .hes extension\n", filename);
    }
    
    /* Create VM and run the file */
    hm_vm* vm = hm_vm_create();
    
    if (!hm_vm_load_file(vm, filename)) {
        printf("Error: Failed to load file: %s\n", filename);
        hm_vm_destroy(vm);
        return 1;
    }
    
    if (!hm_vm_run(vm)) {
        printf("Error: Failed to execute: %s\n", filename);
        hm_vm_destroy(vm);
        return 1;
    }
    
    hm_vm_destroy(vm);
    return 0;
}
