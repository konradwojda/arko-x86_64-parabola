#include "draw_bitmap.h"
#include "math.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

GtkAdjustment* param_a;
GtkAdjustment* param_b;
GtkAdjustment* param_c;
GtkAdjustment* param_s;
GtkImage* gtk_bitmap;



#define OUTPUT_FILE_NAME "output.bmp"

#define BMP_HEADER_SIZE 54

#define BMP_PIXEL_OFFSET 54

#define BMP_PLANES 1

#define BMP_BPP_TYPE 24

#define BMP_HORIZONTAL_RES 500

#define BMP_VERTICAL_RES 500

#define BMP_DIB_HEADER_SIZE 40

void draw_bitmap(unsigned char* bitmap, double a, double b, double c, double s);

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
    uint32_t horizontal_res;
    uint32_t vertical_res;
    uint32_t color_plette;
    uint32_t important_colors;
} BmpHeader;

void init_bmp_header(BmpHeader *header)
{
    header->sig_b = 'B';
    header->sig_m = 'M';
    header->reserved = 0;
    header->pixel_offset = BMP_PIXEL_OFFSET;
    header->header_size = BMP_DIB_HEADER_SIZE;
    header->planes = BMP_PLANES;
    header->bpp_type = BMP_BPP_TYPE;
    header->compression = 0;
    header->image_size = 0;
    header->horizontal_res = BMP_HORIZONTAL_RES;
    header->vertical_res = BMP_VERTICAL_RES;
    header->color_plette = 0;
    header->important_colors = 0;
}

void write_bytes_to_bmp(unsigned char *buffer, size_t size)
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



unsigned char *generate_white_bitmap(unsigned int width, unsigned int height, size_t *output_size)
{
    unsigned int row_size = (width * 3 + 3) & ~3;
    *output_size = row_size * height + BMP_HEADER_SIZE;
    unsigned char *bitmap = (unsigned char *) malloc(*output_size);

    BmpHeader header;
    init_bmp_header(&header);
    header.size = *output_size;
    header.width = width;
    header.height = height;

    memcpy(bitmap, &header, BMP_HEADER_SIZE);
    int i = BMP_HEADER_SIZE;
    for (BMP_HEADER_SIZE; i < *output_size; ++i)
    {
        bitmap[i] = 0xff;
    }
    return bitmap;
}

void exit_exit()
{
    gtk_main_quit();
}


void draw()
{
    double A = gtk_adjustment_get_value(param_a);
    double B = gtk_adjustment_get_value(param_b);
    double C = gtk_adjustment_get_value(param_c);
    double S = gtk_adjustment_get_value(param_s);
    size_t bmp_size = 0;
    unsigned char *bmp_buffer = generate_white_bitmap(512, 512, &bmp_size);
    printf("%d\n", sizeof(BmpHeader));
    printf("%d\n", bmp_size);
    draw_bitmap(bmp_buffer, A, B, C, S);
    write_bytes_to_bmp(bmp_buffer, bmp_size);
    free(bmp_buffer);
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