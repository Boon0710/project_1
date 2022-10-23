#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char* uc_arrayNew_1d(int _size){
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char* background_subtraction(unsigned char* image, unsigned char* image1, int width, int height, int channel){
    unsigned char * new_image = uc_arrayNew_1d(width * height);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int sum  = 0;
            for(int k = 0; k < channel; k++){
                sum += abs(image1[i * width * channel + j * channel + k] - image[i * width * channel + j * channel + k]);
            }
            if((sum/3) < 40){
                new_image[i * width + j] = 0;
            } else{
                new_image[i * width + j] = 255;
            }
        }
    }
    return new_image;
}

unsigned char* final(unsigned char* n_image, unsigned char* foreground, unsigned char* weather_forecast, int width, int height, int channel){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(n_image[i * width + j] == 255){
                for(int k = 0; k < channel; k++){
                    weather_forecast[i * width * channel + j * channel + k] = foreground[i * width * channel + j * channel + k];
                }
            }
        }
    }
    return weather_forecast;
}

int main(){
    char save_path[] = "./image/foreground_new.png";
    char final_result[] = "./image/weather_forecast_final.png";

    int width, height, channel;
    unsigned char *image = stbi_load("./image/background.jpg", &width, &height, &channel, 0); 
    if(image == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channel);

    
    unsigned char *image1 = stbi_load("./image/foreground.jpg", &width, &height, &channel, 0); 
    if(image1 == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channel);

    unsigned char *image2 = stbi_load("./image/weather_forecast.jpg", &width, &height, &channel, 0); 
    if(image2 == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channel);

    unsigned char* n_image = background_subtraction(image, image1, width, height, channel);
    unsigned char* final_res = final(n_image, image1, image2, width, height, channel);
    
    stbi_write_png(save_path, width, height, 1, n_image, width);
    stbi_write_png(final_result, width, height, 3, final_res, width * channel);
    
    printf("New image saved to %s\n", save_path);
    printf("New image saved to %s\n", final_result);
}