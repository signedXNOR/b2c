#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <raylib.h>

bool santamaria = 0;
bool verbose = 0;

unsigned int pixelcnt = 1;
unsigned int width = 10;

Color * pixels;
unsigned int scale = 1;
unsigned int prefferedWidth = 1300;

int main(int argc, char ** argv)
{
    for (int i = 0; i<argc; i++)
    {
        if ((!strcmp(argv[i], "-s") || !strcmp(argv[i], "--scale")) && argv[i+1] != NULL)
        {
            scale = atoi(argv[i+1]);
        }
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--display"))
        {
            santamaria = 1;
        }
        if ((!strcmp(argv[i], "-w") || !strcmp(argv[i], "--width")) && argv[i+1] != NULL)
        {
            prefferedWidth = atoi(argv[i+1]);
        }
        if ((!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")))
        {
            verbose = true;
        }
        if ((!strcmp(argv[i], "-r") || !strcmp(argv[i], "--reverse")))
        {
            /* to do */
        }
    }

    pixels = calloc(1, sizeof(Color));
    unsigned int i = 0;
    while (true)
    {
        int c = getc(stdin);
        if (c >= 0)
        {
            if (verbose && i % 32768 == 0) printf("i: %d\n", i);
            if (i % 3 == 0 && i != 0) {
                pixelcnt++;
                pixels = realloc(pixels, pixelcnt*sizeof(Color));
            }
            if (i%3==0) pixels[(int)((i+1*(i==0))/3)].r = c;
            if (i%3==1) pixels[(int)((i+1*(i==0))/3)].g = c;
            if (i%3==2) pixels[(int)((i+1*(i==0))/3)].b = c;
            pixels[(int)((i+1*(i==0))/3)].a = 0xff;
            i++;
        } else {
            switch (i%3)
            {
                case 0:
                    pixels[(int)((i+1*(i==0))/3)].g = 0;
                    pixels[(int)((i+1*(i==0))/3)].b = 0;
                    break;
                case 1:
                    pixels[(int)((i+1*(i==0))/3)].b = 0;
            }
            break;
        }
    }

    width = 1;
    while (pixelcnt > width*width)
    {
        width++;
    }
    pixels = realloc(pixels, width*width*sizeof(Color));
    for (unsigned int l = 0; l < width*width-pixelcnt; l++)
    {
        pixels[pixelcnt+l].r = 0;
        pixels[pixelcnt+l].g = 0;
        pixels[pixelcnt+l].b = 0;
        pixels[pixelcnt+l].a = 255;
    }

    if (santamaria)
    {
        scale = (prefferedWidth/width); // scale works weird, need to rework
        SetTraceLogLevel(LOG_ERROR);

        InitWindow(width*scale, width*scale, "0x4e to rgb");
        BeginDrawing();
        ClearBackground(BLACK);
        
        for (unsigned int j = 0; j<width; j++)
        {
            for (unsigned int i = 0; i<width; i++)
            {
                DrawRectanglePro(
                    (Rectangle)
                    {
                        i*scale,
                        j*scale,
                        width*scale,
                        width*scale
                    },
                    (Vector2){0.0f, 0.0f},
                    0.0f,
                    pixels[i+j*width]); 
            }
        }
        EndDrawing();
        free(pixels);
        while (!WindowShouldClose()) { /* nada */ } 
        CloseWindow();
    } else {
        ExportImage(
        (Image)
        {
            pixels,
            width,
            width,
            1,
            PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        },
        "color-(previously-bytes).png");
        free(pixels);
    }
}