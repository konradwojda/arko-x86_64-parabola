#include "draw_bitmap.h"
#include "math.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

/*
Params for Gtk GUI
*/
GtkAdjustment* param_a;
GtkAdjustment* param_b;
GtkAdjustment* param_c;
GtkAdjustment* param_s;
GtkImage* gtk_bitmap;


/*
Constants used for operate on BMP files
*/
#define OUTPUT_FILE_NAME "output.bmp"

#define HEADER_SIZE 54

#define PIXEL_OFFSET 54

#define PLANES 1

#define BPP_TYPE 24

#define HORIZONTAL_RESOLUTION 500

#define VERTICAL_RESOLUTION 500

#define DIB_HEADER_SIZE 40

void draw_bitmap(unsigned char* bitmap, double a, double b, double c, double s);

/*
Struct to represent basic BMP Header
*/
typedef struct {
    unsigned char sig_b;
    unsigned char sig_m;
    uint32_t size;
    uint32_t reserved;
    uint32_t pixel_offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp_type;
    uint32_t compression;
    uint32_t image_size;
    uint32_t horizonal_res;
    uint32_t vertical_res;
    uint32_t color_plette;
    uint32_t important_colors;
} BmpHeader;

/*
Initializes BMP Header with default values
*/
void initialize_bmp_header(BmpHeader *header)
{
    header->sig_b = 'B';
    header->sig_m = 'M';
    header->reserved = 0;
    header->pixel_offset = PIXEL_OFFSET;
    header->header_size = DIB_HEADER_SIZE;
    header->planes = PLANES;
    header->bpp_type = BPP_TYPE;
    header->compression = 0;
    header->image_size = 0;
    header->horizonal_res = HORIZONTAL_RESOLUTION;
    header->vertical_res = VERTICAL_RESOLUTION;
    header->color_plette = 0;
    header->important_colors = 0;
}

/*
Writes given buffer to BMP file
*/
void write_buffer_to_bmp(unsigned char *buffer, size_t size)
{
    FILE *file;

    file = fopen(OUTPUT_FILE_NAME, "wb");
    if (file == NULL)
    {
        printf("Error");
        exit(-1);
    }
    fwrite(buffer, 1, size, file);
    fclose(file);
}


/*
Generates empty bitmap of given width and height and fills it with white color.
*/
unsigned char *generate_white_bitmap(unsigned int width, unsigned int height, size_t *output_size)
{
    //Calculate row size
    unsigned int row_size = (width * 3 + 3) & ~3;

    //Calculate output size
    *output_size = row_size * height + HEADER_SIZE;

    //Allocs memory for bitmap
    unsigned char *bitmap = (unsigned char *) malloc(*output_size);

    //Creates and initializes header
    BmpHeader header;
    initialize_bmp_header(&header);
    header.size = *output_size;
    header.width = width;
    header.height = height;

    //Copies bytes from header to bitmap
    memcpy(bitmap, &header, HEADER_SIZE);

    //Fills bitmap with white color
    for (int i = HEADER_SIZE; i < *output_size; ++i)
    {
        bitmap[i] = 0xff;
    }
    return bitmap;
}

/*
Exit function for GUI
*/
void exit_exit()
{
    gtk_main_quit();
}

/*
Function which is called on GUI param change
*/
void draw()
{
    //Get the params
    double A = gtk_adjustment_get_value(param_a);
    double B = gtk_adjustment_get_value(param_b);
    double C = gtk_adjustment_get_value(param_c);
    double S = gtk_adjustment_get_value(param_s);

    //Create an empty bitmap
    size_t bmp_size = 0;

    //width and height must be even
    unsigned char *bmp_buffer = generate_white_bitmap(640, 360, &bmp_size);

    //Debug prints
    // printf("%d\n", sizeof(BmpHeader));
    // printf("%d\n", bmp_size);

    //Call assembly function to draw a parabola of given params
    draw_bitmap(bmp_buffer, A, B, C, S);

    //Write buffer to file
    write_buffer_to_bmp(bmp_buffer, bmp_size);

    //Dealloc memory
    free(bmp_buffer);

    //View an image in GUI
    gtk_image_set_from_file(gtk_bitmap, OUTPUT_FILE_NAME);
}

int main(int argc, char* argv[])
{
    GtkBuilder* builder;
    GtkWidget* window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ui.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_builder_connect_signals(builder, NULL);

    param_a = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "param_a"));
    param_b = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "param_b"));
    param_c = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "param_c"));
    param_s = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "param_s"));

    gtk_bitmap = GTK_IMAGE(gtk_builder_get_object(builder, "bitmap"));

    g_object_unref(builder);
    draw();

    gtk_widget_show(window);
    gtk_main();

    return 0;

}