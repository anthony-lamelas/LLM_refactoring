// Refactored code with clear comments
#include <stdio.h>
#include <math.h>

#define ITERATE(x) for (index = 0; index < x; ++index)

// Declare global variables with meaningful names
float temperature = 240, buoyancy = 0.53, velocity, offset[4426112];
float red, green, blue, density, time_step, gravity = 0;
char display_buffer[60737];
int width = 80, height = 79, depth = 64, levels = 40, base_index = 551368;
int max_particles = 6400, particle_index = 15168, buffer_size = 949, max_velocity = 12;
int constant_factor = 0x1fbb4000;
float *get_offset(int time, int x, int y, int level);
float calculate_brightness(float x, float y, float z);
void normalize_vector(float x, float y, float z, float *nx, float *ny, float *nz);
void smooth(int x, int source);
float *get_density(int offset);
float absolute_value(float value);
void swap_offsets(int x, int y);
void adjust_value(float *value);
void update_particles(int dimension, int source, int velocity_x, int velocity_y, int velocity_z);
void apply_velocity(int offset, int source, int velocity_x, int velocity_y, int velocity_z);
void compute_flux(int x, int y, int z, int source);
void update_velocity(int dimension, int source, int velocity_x, int velocity_y, int velocity_z);
void apply_changes(int x, int source, int step_x, int step_y, int step_z);
void visualize();

int main() {
    int total_elements = max_particles * width;
    int buffer_length = particle_index + levels * 8;
    int index, current_time, x, y, z, particle_count, buffer_offset, element_index;
    float x_pos, y_pos, z_pos, normalized_x, normalized_y, normalized_z;
    
    // Initialize display buffer
    display_buffer[buffer_size * height] = '\0';

    // Initialize offset array
    ITERATE(buffer_length) {
        offset[index] = 0;
    }

    // Initialize the grid
    for (current_time = 0; current_time < height; ++current_time) {
        display_buffer[current_time * buffer_size + width * max_velocity] = '\n';
        for (x = 0; x < width;) {
            normalize_vector(x++ - 40, current_time - 32, -145, &blue, &green, &red);
            *get_density(0) = blue;
            *get_density(1) = green;
            *get_density(2) = red;
        }
    }

    // Simulation loop
    for (;;) {
        // Reset specific elements in the offset array
        ITERATE(levels) {
            offset[particle_index + 3 * levels + index] = 0;
            offset[particle_index + 4 * levels + index] = 0;
            offset[particle_index + 5 * levels + index] = 0;
            offset[particle_index + 7 * levels + index] = 0;
        }

        // Update particles and velocity
        ITERATE(total_elements) {
            element_index = index % max_particles;
            current_time = element_index % width + 1;
            x = element_index / width + 1;
            y = index / max_particles + 1;
            x_pos = current_time - depth;
            y_pos = y - depth;
            density = x_pos * x_pos + y_pos * y_pos;
            int density_index = constant_factor + (*(int *)&density >> 1);
            float density_value = *(float *)&density_index;
            if (x < 13 && x > 10) {
                if (density_value < 2) {
                    *get_offset(current_time, x, y, 4) = 1.5;
                    gravity = gravity + (x / width);
                    float gravity_sq = gravity * gravity;
                    velocity = gravity - (gravity_sq * gravity) / 6 + (gravity_sq * gravity_sq * gravity) / 120;
                    density = 1 - velocity / 2 + (velocity * velocity) / 24;
                    *get_offset(current_time, x, y, 3) = velocity / 2 + 0.0275;
                    *get_offset(current_time, x, y, 5) = density * 0.7 - 0.35;
                }
            }
            if (x < 2 && density_value < 3) {
                *get_offset(current_time, x, y, 7) = 0.1;
            }
        }

        // Perform smoothing and swapping
        smooth(0, 3);
        smooth(1, 4);
        smooth(2, 5);
        swap_offsets(5, 2);
        swap_offsets(4, 1);
        swap_offsets(3, 0);

        // Compute flux and update particles
        compute_flux(3, 4, 5, 1);
        update_particles(1, 4, 3, 4, 5);
        update_particles(0, 3, 3, 4, 5);
        update_particles(2, 5, 3, 4, 5);
        compute_flux(0, 1, 2, 4);

        // Visualize the result
        visualize();
    }
    
    return 0;
}

// Function to get the offset pointer
float *get_offset(int time, int x, int y, int level) {
    return &offset[(time + 82 * x + 6724 * y) + (particle_index + level * levels)];
}

// Function to calculate brightness
float calculate_brightness(float x, float y, float z) {
    x = x * x + y * y + z * z;
    int brightness_index = constant_factor + (*(int *)&x >> 1);
    return *(float *)&brightness_index;
}

// Function to normalize a vector
void normalize_vector(float x, float y, float z, float *nx, float *ny, float *nz) {
    float magnitude = calculate_brightness(x, y, z);
    *nx = x / magnitude;
    *ny = y / magnitude;
    *nz = z / magnitude;
}

// Function to smooth the values
void smooth(int x, int source) {
    ITERATE(levels) {
        offset[particle_index + x * levels + index] += 0.4 * offset[particle_index + source * levels + index];
    }
}

// Function to get density pointer
float *get_density(int offset) {
    return &offset[(time_step * width + x) * 3 + offset];
}

// Function to get absolute value
float absolute_value(float value) {
    return value < 0 ? -value : value;
}

// Function to swap offsets
void swap_offsets(int x, int y) {
    ITERATE(total_elements) {
        int element_index = index % max_particles;
        time_step = element_index % width + 1;
        x = element_index / width + 1;
        y = index / max_particles + 1;
        *get_offset(time_step, x, y, x) = *get_offset(time_step, x, y, y);
    }
}

// Function to adjust value within a range
void adjust_value(float *value) {
    *value = *value < 0.5 ? 0.5 : *value > 85 ? 85 : *value;
}

// Function to update particles
void update_particles(int dimension, int source, int velocity_x, int velocity_y, int velocity_z) {
    ITERATE(total_elements) {
        int element_index = index % max_particles;
        time_step = element_index % width + 1;
        x = element_index / width + 1;
        y = index / max_particles + 1;
        float u = time_step - width * 0.4 * *get_offset(time_step, x, y, velocity_x);
        float j = x - width * 0.4 * *get_offset(time_step, x, y, velocity_y);
        float e = y - width * 0.4 * *get_offset(time_step, x, y, velocity_z);
        adjust_value(&u);
        adjust_value(&j);
        adjust_value(&e);
        float u_fraction = u - (int)u;
        float j_fraction = j - (int)j;
        float j_complement = 1 - j_fraction;
        float e_fraction = e - (int)e;
        float e_complement = 1 - e_fraction;
        *get_offset(time_step, x, y, dimension) =
            (1 - u_fraction) * (j_complement * e_complement * *get_offset(u, j, e, source) +
                                j_fraction * e_complement * *get_offset(u, 1 + j, e, source) +
                                j_complement * e_fraction * *get_offset(u, j, e + 1, source) +
                                j_fraction * e_fraction * *get_offset(u, 1 + j, 1 + e, source)) +
            u_fraction * (j_complement * e_complement * *get_offset(u + 1, j, e, source) +
                          j_fraction * e_complement * *get_offset(1 + u, 1 + j, e, source) +
                          j_complement * e_fraction * *get_offset(1 + u, j, e + 1, source) +
                          j_fraction * e_fraction * *get_offset(1 + u, j + 1, e + 1, source));
    }
}

// Function to compute flux
void compute_flux(int x, int y, int z, int source) {
    ITERATE(total_elements) {
        int element_index = index % max_particles;
        time_step = element_index % width + 1;
        x = element_index / width + 1;
        y = index / max_particles + 1;
        *get_offset(time_step, x, y, source) = -1.0 / 3 * (
            (*get_offset(1 + time_step, x, y, x) - *get_offset(time_step - 1, x, y, x)) / width +
            (*get_offset(time_step, x + 1, y, y) - *get_offset(time_step, x - 1, y, y)) / width +
            (*get_offset(time_step, x, 1 + y, z) - *get_offset(time_step, x, y - 1, z)) / width
        );
    }
}

// Function to update velocity
void update_velocity(int dimension, int source, int velocity_x, int velocity_y, int velocity_z) {
    ITERATE(total_elements) {
        int element_index = index % max_particles;
        time_step = element_index % width + 1;
        x = element_index / width + 1;
        y = index / max_particles + 1;
        apply_velocity(dimension, source, velocity_x, velocity_y, velocity_z);
    }
}

// Function to apply changes to velocity
void apply_changes(int x, int source, int step_x, int step_y, int step_z) {
    *get_offset(time_step, x, y, source) -= 40 * (*get_offset(time_step + step_x, x + step_y, y + step_z, x) -
                                                  *get_offset(time_step - step_x, x - step_y, y - step_z, x));
}

// Function to visualize the result
void visualize() {
    for (int t = 0; t < max_particles; ++t) {
        for (int i = 0; i < 5; ++i) {
            int buffer_offset = (t * buffer_size + i * max_velocity) + 8;
            for (int a = 0; a < max_velocity; ++a) {
                display_buffer[buffer_offset + a - 8] = ' ';
            }
        }
    }
    puts(display_buffer);
}