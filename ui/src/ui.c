#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "../../filters/include/filters.h"
#include "../../solver/include/solver.h"


GtkWidget *image = NULL; // Global variable to store the GtkImage widget
GtkWidget *buttonBox = NULL; // Global variable to store the button box
gboolean buttonsDisplayed = FALSE; // Flag to track whether buttons have been displayed
GdkPixbuf *originalPixbuf = NULL;  // Global variable to store the original image
gboolean originalImageLoaded = FALSE;  // Flag to check if the original image has been loaded
// Global variables
int columns;
int rows;
GtkWidget *window;

//for the grid 
GtkWidget *clickedButton = NULL;

// Assume you have a 9x9 grid of buttons
GtkWidget *grid[9][9];

// Declare sudoku_grid as a global variable


/* je dois changer le fichier text en une matrice de int 
ca permettra de l'afficher correctement lorsque je clique sur le bouton "grid"*/
int sudoku_grid[9][9] = {0}; 

// Callback function to handle button click
void on_button_clicked(GtkWidget *widget, gpointer data);

// Placeholder actions for each button click
void on_gaussian_button_clicked(GtkWidget *widget, gpointer data);
void on_invert_button_clicked(GtkWidget *widget, gpointer data);
void on_threshold_button_clicked(GtkWidget *widget, gpointer data);
void on_grayscale_button_clicked(GtkWidget *widget, gpointer data);
void on_sobel_button_clicked(GtkWidget *widget, gpointer data);
void on_hough_button_clicked(GtkWidget *widget, gpointer data);
void on_rotate_button_clicked(GtkWidget *widget, gpointer data);
void on_original_image_button_clicked(GtkWidget *widget, gpointer data);
void on_solve_button_clicked(GtkWidget *widget, gpointer data);
void on_grid_button_clicked(GtkWidget *widget, gpointer data);  // New function for the "Grid" button
void on_window_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);

// Callback function to handle the "destroy" signal
void on_window_destroy(GtkWidget *widget, gpointer data);

// Callback function for the file chooser dialog response
void on_file_chooser_response(GtkWidget *dialog, gint response_id, gpointer user_data);

// Callback function to show the file chooser dialog
void on_open_button_clicked(GtkWidget *widget, gpointer data);

// Function to add buttons on the right side of the UI
void display_additional_buttons(GtkWidget *window);

// Function to reset the main window's content
void on_ok_button_clicked(GtkButton *button, gpointer data);

void on_white_button_clicked(GtkWidget *widget, gpointer data);


SDL_Surface *gdk_pixbuf_to_sdl_surface(GdkPixbuf *pixbuf);

//function to load the image
void load_image(const char *file_path, GtkWidget *image_widget) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);

   if (pixbuf == NULL) {
        // Print an error message with details and return
        g_printerr("Error loading image from file: %s\n", file_path);
        return;
    }

    // Set the loaded image in the GtkImage widget
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);

    // Unreference the pixbuf to free resources
    g_object_unref(pixbuf);
}


//function to load the css file 
void load_css(GtkWidget *widget, const char *css_file_path) {
    // Load the CSS file
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    GError *error = NULL;

    if (gtk_css_provider_load_from_path(cssProvider, css_file_path, &error)) {
        // Apply the CSS styling to the widget
        GtkStyleContext *styleContext = gtk_widget_get_style_context(widget);
        gtk_style_context_add_provider(styleContext, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        // The caller is responsible for freeing the allocated memory
        g_object_unref(cssProvider);
    } else {
        // Handle the error (e.g., print an error message)
        fprintf(stderr, "Error loading CSS file: %s\n", error->message);
        g_error_free(error);
    }
}


int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Train Sudoku Challenger");
    gtk_widget_set_size_request(window, 1000, 800);
    //gtk_widget_set_name(window, "window");


    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "style_windows.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    // Connect the "destroy" signal to the callback function
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Connect the "configure-event" signal to handle window resizing
    g_signal_connect(window, "configure-event", G_CALLBACK(on_window_configure_event), NULL);

    // Create an "Open" button
    GtkWidget *open_button = gtk_button_new_with_label("Open");
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_button_clicked), window);

    // Create a button box to hold the "Open" button
    GtkWidget *box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);
    gtk_container_add(GTK_CONTAINER(box), open_button);

    // Create a grid to position the buttons on the right
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), box, 1, 0, 1, 1);
   // gtk_grid_attach(GTK_GRID(grid), grid_button, 1, 1, 1, 1);

    // Create a GtkImage widget with a fixed size
    image = gtk_image_new();
    gtk_image_set_pixel_size(GTK_IMAGE(image), 400); // Adjust the initial pixel size as needed
    gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 1, 2);

    // Create a button box to hold additional buttons
    buttonBox = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_set_border_width(GTK_CONTAINER(buttonBox), 10);

    // Create a new container to hold both the existing grid and the new button box
    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(container), grid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(container), buttonBox, FALSE, FALSE, 0);

    // Set the container as the child of the main window
    gtk_container_add(GTK_CONTAINER(window), container);

    // Display all components
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}

// Callback function to handle window resizing
void on_window_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
    // Get the current size of the window
    int window_width = event->width;
    int window_height = event->height;

    // Load the current image
    GdkPixbuf *current_pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    if (current_pixbuf != NULL) {
        // Get the original size of the image
        int original_width = gdk_pixbuf_get_width(current_pixbuf);
        int original_height = gdk_pixbuf_get_height(current_pixbuf);

        // Calculate the aspect ratio of the image
        double aspect_ratio = (double)original_width / (double)original_height;

        // Calculate the new size for the image to fill 2/3 of the window
        int target_width = window_width * 2 / 3;
        int target_height = target_width / aspect_ratio;

        // Resize the image
        GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(current_pixbuf, target_width, target_height, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), resized_pixbuf);
        g_object_unref(resized_pixbuf);
    }
}


//depending on which button is clicked on the ui
// Callback function to handle button click
void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("%s Button Clicked!\n", button_label);

    // Placeholder actions for each button click
    if (strcmp(button_label, "Open") == 0) {
        // Handle "Open" button click
        on_open_button_clicked(widget, data);
    } else if (strcmp(button_label, "Gaussian") == 0) {
        // Handle "Gaussian" button click
        on_gaussian_button_clicked(widget, data);
    } else if (strcmp(button_label, "Invert") == 0) {
        // Handle "Invert" button click
        on_invert_button_clicked(widget, data);
    } else if (strcmp(button_label, "Threshold") == 0) {
        // Handle "Threshold" button click
        on_threshold_button_clicked(widget, data);
    } else if (strcmp(button_label, "Grayscale") == 0) {
        // Handle "Grayscale" button click
        on_grayscale_button_clicked(widget, data);
    } else if (strcmp(button_label, "Sobel") == 0) {
        // Handle "Sobel" button click
        on_sobel_button_clicked(widget, data);
    } else if (strcmp(button_label, "Hough") == 0) {
        // Handle "Hough" button click
        on_hough_button_clicked(widget, data);
    } else if (strcmp(button_label, "Rotate") == 0) {
        // Handle "Rotate" button click
        on_rotate_button_clicked(widget, data);
    } else if (strcmp(button_label, "Original_Image") == 0) {
        // Handle "Original Image" button click
        on_original_image_button_clicked(widget, data);
    } else if (strcmp(button_label, "Solve") == 0) {
        // Handle "Solve" button click
        on_solve_button_clicked(widget, data);
    } else if (strcmp(button_label, "Grid") == 0) {
        // Handle "Grid" button click
        on_grid_button_clicked(widget, data);
    }
}






//image processing



// Placeholder actions for each button click
// Takes a grayscale image.
//2
void on_gaussian_button_clicked(GtkWidget *widget, gpointer data) {
    //print on the terminal 
    g_print("Gaussian Button Clicked!\n");

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    // Convert GdkPixbuf to SDL_Surface
    SDL_Surface *inputSurface = gdk_pixbuf_to_sdl_surface(pixbuf);

    // Apply the Grayscale filter
    SDL_Surface *resultSurface = gaussian_filter(inputSurface);

    // Save the result to a file
    const char *outputPath = "../ui/output.png";
    if (SDL_SaveBMP(resultSurface, outputPath) != 0) {
        // Print an error message with details
        g_printerr("Error: couldn't save the result image. SDL_Error: %s\n", SDL_GetError());

        // Free the result surface
        SDL_FreeSurface(resultSurface);

        // Free the input surface
        SDL_FreeSurface(inputSurface);

        // Return without trying to load the image
        return;
    }

    // Free the result surface
    SDL_FreeSurface(resultSurface);

    // Free the input surface
    SDL_FreeSurface(inputSurface);

    // Remove the currently displayed image
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), NULL);

    // Display the result in the GtkImage widget
    load_image(outputPath, image);
}

// after the threshold 
//4
void on_invert_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Invert Button Clicked!\n");

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    // Convert GdkPixbuf to SDL_Surface
    SDL_Surface *inputSurface = gdk_pixbuf_to_sdl_surface(pixbuf);

    // Apply the Grayscale filter
    SDL_Surface *resultSurface = invert_filter(inputSurface);

    // Save the result to a file
    const char *outputPath = "../ui/output.png";
    if (SDL_SaveBMP(resultSurface, outputPath) != 0) {
        // Print an error message with details
        g_printerr("Error: couldn't save the result image. SDL_Error: %s\n", SDL_GetError());

        // Free the result surface
        SDL_FreeSurface(resultSurface);

        // Free the input surface
        SDL_FreeSurface(inputSurface);

        // Return without trying to load the image
        return;
    }

    // Free the result surface
    SDL_FreeSurface(resultSurface);

    // Free the input surface
    SDL_FreeSurface(inputSurface);

    // Remove the currently displayed image
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), NULL);

    // Display the result in the GtkImage widget
    load_image(outputPath, image);
}

//3
void on_threshold_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Threshold Button Clicked!\n");

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    // Convert GdkPixbuf to SDL_Surface
    SDL_Surface *inputSurface = gdk_pixbuf_to_sdl_surface(pixbuf);

    // Apply the Grayscale filter
    SDL_Surface *resultSurface = threshold_filter(inputSurface);

    // Save the result to a file
    const char *outputPath = "../ui/output.png";
    if (SDL_SaveBMP(resultSurface, outputPath) != 0) {
        // Print an error message with details
        g_printerr("Error: couldn't save the result image. SDL_Error: %s\n", SDL_GetError());

        // Free the result surface
        SDL_FreeSurface(resultSurface);

        // Free the input surface
        SDL_FreeSurface(inputSurface);

        // Return without trying to load the image
        return;
    }

    // Free the result surface
    SDL_FreeSurface(resultSurface);

    // Free the input surface
    SDL_FreeSurface(inputSurface);

    // Remove the currently displayed image
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), NULL);

    // Display the result in the GtkImage widget
    load_image(outputPath, image);
}
//1
void on_grayscale_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Grayscale Button Clicked!\n");

    // Get the current image from the GtkImage widget
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    // Convert GdkPixbuf to SDL_Surface
    SDL_Surface *inputSurface = gdk_pixbuf_to_sdl_surface(pixbuf);

    // Apply the Grayscale filter
    SDL_Surface *resultSurface = grayscale_filter(inputSurface);

    // Save the result to a file
    const char *outputPath = "../ui/output.png";
    if (SDL_SaveBMP(resultSurface, outputPath) != 0) {
        // Print an error message with details
        g_printerr("Error: couldn't save the result image. SDL_Error: %s\n", SDL_GetError());

        // Free the result surface
        SDL_FreeSurface(resultSurface);

        // Free the input surface
        SDL_FreeSurface(inputSurface);

        // Return without trying to load the image
        return;
    }

    // Free the result surface
    SDL_FreeSurface(resultSurface);

    // Free the input surface
    SDL_FreeSurface(inputSurface);

    // Remove the currently displayed image
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), NULL);

    // Display the result in the GtkImage widget
    load_image(outputPath, image);
}

//convert a pixbuf( gtk for representing image) to a sdl_surface for the filters
SDL_Surface *gdk_pixbuf_to_sdl_surface(GdkPixbuf *pixbuf) {
    // Get information from GdkPixbuf
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int channels = gdk_pixbuf_get_n_channels(pixbuf);

    // Create an SDL_Surface
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, channels * 8, 0, 0, 0, 0);
    if (!surface) {
        g_printerr("Error: couldn't create SDL surface. SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Copy pixels from GdkPixbuf to SDL_Surface
    SDL_LockSurface(surface);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    memcpy(surface->pixels, pixels, height * rowstride);
    SDL_UnlockSurface(surface);

    return surface;
}


//5
void on_sobel_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Sobel Button Clicked!\n");

    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    // Convert GdkPixbuf to SDL_Surface
    SDL_Surface *inputSurface = gdk_pixbuf_to_sdl_surface(pixbuf);

    // Apply the Grayscale filter
    SDL_Surface *resultSurface = sobel_filter(inputSurface);

    // Save the result to a file
    const char *outputPath = "../ui/output.png";
    if (SDL_SaveBMP(resultSurface, outputPath) != 0) {
        // Print an error message with details
        g_printerr("Error: couldn't save the result image. SDL_Error: %s\n", SDL_GetError());

        // Free the result surface
        SDL_FreeSurface(resultSurface);

        // Free the input surface
        SDL_FreeSurface(inputSurface);

        // Return without trying to load the image
        return;
    }

    // Free the result surface
    SDL_FreeSurface(resultSurface);

    // Free the input surface
    SDL_FreeSurface(inputSurface);

    // Remove the currently displayed image
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), NULL);

    // Display the result in the GtkImage widget
    load_image(outputPath, image);
}

//6
void on_hough_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Hough Button Clicked!\n");


    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

    // Convert GdkPixbuf to SDL_Surface
    SDL_Surface *inputSurface = gdk_pixbuf_to_sdl_surface(pixbuf);

    // Apply the Grayscale filter
    SDL_Surface *resultSurface = hough_transform_filter(inputSurface);

    // Save the result to a file
    const char *outputPath = "../ui/output.png";
    if (SDL_SaveBMP(resultSurface, outputPath) != 0) {
        // Print an error message with details
        g_printerr("Error: couldn't save the result image. SDL_Error: %s\n", SDL_GetError());

        // Free the result surface
        SDL_FreeSurface(resultSurface);

        // Free the input surface
        SDL_FreeSurface(inputSurface);

        // Return without trying to load the image
        return;
    }

    // Free the result surface
    SDL_FreeSurface(resultSurface);

    // Free the input surface
    SDL_FreeSurface(inputSurface);

    // Remove the currently displayed image
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), NULL);

    // Display the result in the GtkImage widget
    load_image(outputPath, image);
}


void on_rotate_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Rotate Button Clicked!\n");
}



//button to display the original image 
void on_original_image_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Original Image Button Clicked!\n");

    // Check if the original image is loaded
    if (originalImageLoaded) {
        // Create a new top-level window
        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Original Image Copy");

        // Create a new GtkImage to display the copy of the original image
        GdkPixbuf *copy_pixbuf = gdk_pixbuf_copy(originalPixbuf);
        GtkWidget *image_copy = gtk_image_new_from_pixbuf(copy_pixbuf);

        // Create a scrolled window to handle the image if it's larger than the window
        GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                       GTK_POLICY_AUTOMATIC,
                                       GTK_POLICY_AUTOMATIC);

        // Add the image to the scrolled window
        gtk_container_add(GTK_CONTAINER(scrolled_window), image_copy);

        // Add the scrolled window to the main window
        gtk_container_add(GTK_CONTAINER(window), scrolled_window);

        // Set the size of the window
        gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);  // Adjust the size as needed

        // Show all the widgets
        gtk_widget_show_all(window);
    } else {
        // Print a message or handle the case where the original image is not loaded
        g_print("Original image not loaded.\n");
    }
}







//for the grid button click 

//convert a file into a matrix 
void read_and_convert_sudoku(const char *file_path, int sudoku_grid[9][9]) {
    // Read the Sudoku grid from the file
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            char c;
            if (fscanf(file, " %c", &c) == 1) {
                if (c >= '1' && c <= '9') {
                    sudoku_grid[i][j] = c - '0';
                } else {
                    sudoku_grid[i][j] = 0;
                }
            }
        }
    }

    // Close the file
    fclose(file);
}


//function that display the grid into the windows
void display_sudoku_grid(GtkWidget *window, int sudoku_grid[9][9]) {
    // Get the existing grid from the container
    GtkWidget *container = gtk_bin_get_child(GTK_BIN(window));

    // Remove the current image from the container
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), NULL);

    // Create a new grid to hold the white buttons
    GtkWidget *buttonGrid = gtk_grid_new();

    // Calculate the number of rows and columns for the grid (e.g., 9x9)
    rows = 9;
    columns = 9;

    // Calculate the button size based on the available space
    int buttonSize = MIN(
        gtk_widget_get_allocated_width(window) / columns,
        gtk_widget_get_allocated_height(window) / rows
    );

    // Create and add clickable buttons with labels to the new grid
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // Convert the integer to a string
            char buffer[3];
            snprintf(buffer, sizeof(buffer), "%d", sudoku_grid[i][j]);

            GtkWidget *button = gtk_button_new();
            gtk_widget_set_size_request(button, buttonSize, buttonSize);

            // Set the background color of the button to white
            GdkRGBA color = {1.0, 1.0, 1.0, 1.0}; // White color
            gtk_widget_override_background_color(button, GTK_STATE_FLAG_NORMAL, &color);

            // Create a label with the number from sudoku_grid and add it to the button
            GtkWidget *label = gtk_label_new(buffer);
            gtk_container_add(GTK_CONTAINER(button), label);

            gtk_grid_attach(GTK_GRID(buttonGrid), button, j, i, 1, 1);

            // Connect the button's "clicked" signal to a callback function
            g_signal_connect(button, "clicked", G_CALLBACK(on_white_button_clicked), NULL);
        }
    }

    // Add the new grid to the container
    gtk_container_add(GTK_CONTAINER(container), buttonGrid);

    // Show all components in the window
    gtk_widget_show_all(window);
}


// Callback function to handle the "Grid" button click
void on_grid_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Grid Button Clicked!\n");

    /*donner limage a l'ia et recupurer le resultat et mettre le bon path dans la fonction reand_and_convert*/

    // Convert the file with the sudoku data
    read_and_convert_sudoku("../assets/grids/grid_1", sudoku_grid);

    // Get the current window
    GtkWidget *window = GTK_WIDGET(data);

    // Display the Sudoku grid
    display_sudoku_grid(window, sudoku_grid);
}



// Global variable to store the clicked button

// Callback function for white button click
void on_white_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button Clicked at position: %d\n", GPOINTER_TO_INT(data));

    // Store the clicked button
    clickedButton = widget;

    // Create an entry dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter Number",
        GTK_WINDOW(window),
        GTK_DIALOG_MODAL,
        "OK",
        GTK_RESPONSE_ACCEPT,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL);

    // Create an entry widget
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 1); // Allow only one character

    // Add the entry to the dialog
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry, TRUE, TRUE, 0);

    // Show all components in the dialog
    gtk_widget_show_all(dialog);

    // Run the dialog and wait for the user response
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    // Check if the response is "OK"
    if (result == GTK_RESPONSE_ACCEPT) {
        // Get the entered text from the entry
        const gchar *enteredText = gtk_entry_get_text(GTK_ENTRY(entry));

        // Update the text of the clicked button
        gtk_button_set_label(GTK_BUTTON(clickedButton), enteredText);






        //doit update le changement sur la matrice sudoku_grid
        // Update the corresponding value in the sudoku_grid array
       //int buttonRow = -1, buttonColumn = -1;
        
        //trop d'argument 
        
        //gtk_grid_get_child_at(GTK_GRID(gtk_bin_get_child(GTK_BIN(window))), clickedButton, &buttonColumn, &buttonRow);
        //sudoku_grid[buttonRow][buttonColumn] = atoi(enteredText);
    }

    // Reset the clickedButton variable
    clickedButton = NULL;

    // Destroy the dialog
    gtk_widget_destroy(dialog);
}









//solve button

int find_empty_cell(const int grid[9][9], int *row, int *col)
{
    // For each cell, check if it is equal to 0.
    *row = 0;
    while (*row < 9)
    {
        *col = 0;
        while (*col < 9)
        {
            if (grid[*row][*col] == 0)
            {
                return EXIT_SUCCESS;
            }

            (*col)++;
        }

        (*row)++;
    }

    return EXIT_FAILURE;
}

int is_number_placement_valid(const int grid[9][9], int row, int col,
                              int number)
{
    // Check row and col.
    size_t i = 0;
    while (i < 9)
    {
        if (grid[i][col] == number || grid[row][i] == number)
        {
            return EXIT_FAILURE;
        }

        i++;
    }

    // Check square.
    size_t squareX = row / 3;
    size_t squareY = col / 3;
    for (size_t x = 0; x < 3; x++)
    {
        for (size_t y = 0; y < 3; y++)
        {
            if (grid[squareX * 3 + x][squareY * 3 + y] == number)
            {
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

int solve_grid(int grid[9][9])
{
    // Find the first empty cell.
    int row, col;
    if (find_empty_cell(grid, &row, &col))
    {
        return EXIT_SUCCESS;
    }

    // Test all numbers.
    for (int n = 1; n < 10; n++)
    {
        // If number is correct, place it and call on the next empty cell.
        if (!is_number_placement_valid(grid, row, col, n))
        {
            grid[row][col] = n;
            if (!solve_grid(grid))
            {
                return EXIT_SUCCESS;
            }

            // If it does not work, go back to empty cell.
            grid[row][col] = 0;
        }
    }

    // No solution found.
    return EXIT_FAILURE;
}

const char *file_path = "../ui/result_file.txt";


//reverse de la fonction converti la grille en un fichier text 
void convert_and_write_sudoku(const char *file_path, int sudoku_grid[9][9]) {
    // Open the file for writing
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    // Write the Sudoku grid to the file
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku_grid[i][j] != 0) {
                fprintf(file, "%d", sudoku_grid[i][j]);
            } else {
                fprintf(file, ".");
            }

       // Add spaces after every 3 numbers
            if ((j + 1) % 3 == 0 && j < 8) {
                fprintf(file, " ");
            }
        }

        fprintf(file, "\n");

        // Add newlines after every 3 rows
        if ((i + 1) % 3 == 0 && i < 8) {
            fprintf(file, "\n");
        }
    }

    // Close the file
    fclose(file);
}


void on_solve_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Solve Button Clicked!\n");

    // Load the Sudoku grid from the file

    solve_grid(sudoku_grid);

    // Get the existing container from the window
    GtkWidget *container = gtk_bin_get_child(GTK_BIN(GTK_WIDGET(data)));

    // Remove the existing grid from the container
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(container));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    
    display_sudoku_grid(GTK_WIDGET(data), sudoku_grid);
    
    convert_and_write_sudoku(file_path, sudoku_grid);

}

// Callback function to handle the "destroy" signal
void on_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}









// for the open button
// Callback function for the file chooser dialog response
void on_file_chooser_response(GtkWidget *dialog, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        char *file_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // Load the original image from the selected file
        originalPixbuf = gdk_pixbuf_new_from_file(file_path, NULL);

        // Resize the original image based on the window size
        int window_width, window_height;
        gtk_window_get_size(GTK_WINDOW(user_data), &window_width, &window_height);

        int new_width = window_width / 2;  // Adjust as needed
        int new_height = window_height / 2; // Adjust as needed

        GdkPixbuf *resized_original_pixbuf = gdk_pixbuf_scale_simple(originalPixbuf, new_width, new_height, GDK_INTERP_BILINEAR);

        // Set the resized original image in the GtkImage widget
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), resized_original_pixbuf);

        // Free the resized original pixbuf and file path string
        g_object_unref(resized_original_pixbuf);
        g_free(file_path);

        // Display additional buttons on the right side
        if (!buttonsDisplayed) {
            display_additional_buttons(GTK_WIDGET(user_data));
            buttonsDisplayed = TRUE;
        }

        originalImageLoaded = TRUE;  // Set the flag to indicate that the original image is loaded
    }

    // Destroy the file chooser dialog
    gtk_widget_destroy(dialog);
}


// Callback function to show the file chooser dialog
void on_open_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Open Image",
        GTK_WINDOW(data),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        "Open",
        GTK_RESPONSE_ACCEPT,
        NULL);
    

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        // Handle the response in the callback function
        on_file_chooser_response(dialog, GTK_RESPONSE_ACCEPT, data);
    } else {
        // User canceled the file chooser dialog
        gtk_widget_destroy(dialog);
    }
}









// Function to add buttons on the right side of the UI
void display_additional_buttons(GtkWidget *window) {
    // Create additional buttons
    GtkWidget *gaussianButton = gtk_button_new_with_label("Gaussian");
    GtkWidget *thresholdButton = gtk_button_new_with_label("Threshold");
    GtkWidget *grayscaleButton = gtk_button_new_with_label("Grayscale");
    GtkWidget *invertButton = gtk_button_new_with_label("Invert");
    GtkWidget *sobelButton = gtk_button_new_with_label("Sobel");
    GtkWidget *houghButton = gtk_button_new_with_label("Hough");
    GtkWidget *rotateButton = gtk_button_new_with_label("Rotate");
    GtkWidget *allFilterButton = gtk_button_new_with_label("Original_Image");
    GtkWidget *gridButton = gtk_button_new_with_label("Grid");
    GtkWidget *solveButton = gtk_button_new_with_label("Solve");

    // Connect the buttons' "clicked" signals to the callback function
    g_signal_connect(gaussianButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(thresholdButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(grayscaleButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(invertButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(sobelButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(houghButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(rotateButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(allFilterButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(gridButton, "clicked", G_CALLBACK(on_button_clicked), window);
    g_signal_connect(solveButton, "clicked", G_CALLBACK(on_button_clicked), window);

    // Adjust button size
    int buttonHeight = gtk_widget_get_allocated_height(window) / 10;
    gtk_widget_set_size_request(gaussianButton, -1, buttonHeight);
    gtk_widget_set_size_request(thresholdButton, -1, buttonHeight);
    gtk_widget_set_size_request(grayscaleButton, -1, buttonHeight);
    gtk_widget_set_size_request(invertButton, -1, buttonHeight);
    gtk_widget_set_size_request(sobelButton, -1, buttonHeight);
    gtk_widget_set_size_request(houghButton, -1, buttonHeight);
    gtk_widget_set_size_request(rotateButton, -1, buttonHeight);
    gtk_widget_set_size_request(allFilterButton, -1, buttonHeight);
    gtk_widget_set_size_request(gridButton, -1, buttonHeight);
    gtk_widget_set_size_request(solveButton, -1, buttonHeight);

    // Add buttons to the button box
    gtk_container_add(GTK_CONTAINER(buttonBox), grayscaleButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), gaussianButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), thresholdButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), invertButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), sobelButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), houghButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), rotateButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), allFilterButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), gridButton);
    gtk_container_add(GTK_CONTAINER(buttonBox), solveButton);

    // Display all components
    gtk_widget_show_all(window);
}
