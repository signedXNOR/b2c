#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <raylib.h>

bool santamaria = 0;
bool verbose = 0; 
bool reverse = 0;

Color * pixels;
unsigned int pixelcnt = 1;

unsigned int scale = 1;
unsigned int width = 1;
unsigned int prefferedWidth = 720;

unsigned char * inBytes;
unsigned char * outBytes;
u_int64_t bytecnt = 0;

int main(int argc, char ** argv)
{
    /* args */
    for (int i = 0; i<argc; i++)
    {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--display") || !strcmp(argv[i], "--draw"))
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
            reverse = true;
        }
    }

    if (!reverse)
    {
        pixels = calloc(1, sizeof(Color));

        /* getting the bytes from stdin and building the pixel array */
        unsigned long i = 0;
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
        bytecnt = i;

        /* figuring out the grid size: */
        width = 1;
        while (pixelcnt > width*width) width++;

        /* allocating new pixels in order to fit the grid: */
        pixels = realloc(pixels, width*width*sizeof(Color));

        /* setting said new pixels to 0x000000FF */
        for (unsigned int l = 0; l < width*width-pixelcnt; l++)
        {
            pixels[pixelcnt+l].r = 0;
            pixels[pixelcnt+l].g = 0;
            pixels[pixelcnt+l].b = 0;
            pixels[pixelcnt+l].a = 255;
        }

        // make the image final image
        Image finalImage = (Image){pixels, width, width, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

        // what to do with the final image:
        if (santamaria && prefferedWidth>=width) /* if window rendering option is true and can display */
        {
            SetTargetFPS(60);
            scale = (prefferedWidth/width);
            SetTraceLogLevel(LOG_ERROR);

            InitWindow(width*scale, width*scale, "0x4e to rgb");
            BeginDrawing();
            ClearBackground(BLACK);
            
            DrawTextureEx(LoadTextureFromImage(finalImage), (Vector2){0, 0}, 0.0f, scale, WHITE);
            
            EndDrawing();
            free(pixels);
            while (!WindowShouldClose()) { PollInputEvents(); }
            CloseWindow();
        }
        else if (santamaria && prefferedWidth<width) /* if window rendering option is true but can't display */
        {
            free(pixels);
            printf("Sorry, the image is too big to display at the preffered width!\n");
        }
        else /* if window rendering option is not true*/
        {
            ExportImage
            (
            finalImage,
            "color.png"
            );
            free(pixels);
            FILE * bytecntFile = fopen("bytecnt.boop", "w");
            fwrite(&bytecnt, 8, 1, bytecntFile);
            fclose(bytecntFile);
        }
    }
    else /* this is a mess :] */
    {
        /* getting compressed image data as input: */
        inBytes = calloc(1, 1);
        unsigned long i = 0;
        while (true)
        {
            int c = getc(stdin);
            if (c >= 0)
            {
                // printf("i:%d  c:%c\n", i, c); // debug
                inBytes[i] = c;
                i++;
                inBytes = realloc(inBytes, 1+i);
            }
            else
            {
                break;
            }
        }
        Image inputImage = LoadImageFromMemory(".png", inBytes, i);

        /* using the decompressed byte data */
        unsigned char * inputImageChars = inputImage.data;

        /* getting the size of the byte data */
        FILE * bytecntFile = fopen("bytecnt.boop", "r");
        int l;
        fread(&l, 8, 1, bytecntFile);
        fclose(bytecntFile);
        // printf("%d <- l\n", l); // debnug

        outBytes = malloc(l);

        /* getting rid of useless 0xFF bytes: */
        unsigned int k = 0;
        for (unsigned int j = 0; j < l; j++)
        {
            if ((j+k+1) % 4 == 0) k++;
            outBytes[j] = inputImageChars[j+k];
        }

        /* finally, saving the file: */
        SaveFileData("bytes", outBytes, l);
    }
    
}

/* THANKS SECTION: */
/*  thx to @jefferym on the raylib discord for help with rendering stuff */