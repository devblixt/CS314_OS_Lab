#include <bits/stdc++.h>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <vector>
#include <semaphore.h>
#include <sys/types.h>
#include <thread>
#include <sys/wait.h>

using namespace std;

sem_t sema;
int position[2] = {0,0};

struct Pixel {
    unsigned char r,g,b;
};

typedef std::vector<std::vector<Pixel>> Image;

void readPPM(char *filename,key_t key, int width, int height) {
  std::ifstream inFile(filename, std::ios::binary);
  Image image;
struct Pixel * sharedImage;
  if (inFile.is_open()) {
    std::string line;
    std::getline(inFile, line);

    if (line != "P6") {
      std::cerr << "Error: Invalid PPM file format\n";
      return;
    }

    std::getline(inFile, line);
    int a, b;
    while (line[0] == '#') {
      std::getline(inFile, line);
    }

    std::stringstream ss(line);
    ss >> a >> b;
    //attach memory 
    int shmid = shmget(key, sizeof(struct Pixel)*(height)*(width),0666|IPC_CREAT);

    sharedImage = (struct Pixel*)shmat(shmid,NULL,0);

    std::getline(inFile, line);

    int maxVal = std::stoi(line);

    if (maxVal != 255) {
      std::cerr << "Error: Invalid PPM file format\n";
      return;
    }

    image.resize(height, std::vector<Pixel>(width));

    char pixel[3];
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        inFile.read(pixel, 3);
        image[i][j].r = pixel[0];
        image[i][j].g = pixel[1];
        image[i][j].b = pixel[2];
      }
    }

    inFile.close();
  } else {
    std::cerr << "Error: Could not open input file\n";
    return;
  }
    for(int i = height - 1; i > 0; i--){
        for(int j = 1; j <= width - 1; j++){
            sharedImage[i * width + j] = image[i][j];
        }
    }
}

void T1 (Image &image) {
    //luminosity method for pixel to grayscale
    for (int y=0; y<image.size(); y++) {
        for(int x=0; x <image[y].size();x++) {
            sem_wait(&sema);
            int gray = 0.21*image[y][x].r + 0.72*image[y][x].g + 0.07*image[y][x].b;
            unsigned char graypix = static_cast<unsigned char>(gray);
            image[y][x].r=graypix;
            image[y][x].g=graypix;
            image[y][x].b=graypix;
            position[0] = x+1;
            position[1] = y+1;
            sem_post(&sema);
        }
    }
}

void T2(Image &image) {
  // Define the box filter kernel
  const std::vector<std::vector<double>> kernel = {
      {1.0 / 4, 1.0 / 4, 1.0 / 4},
      {1.0 / 4, 1.0 / 4, 1.0 / 4},
      {1.0 / 4, 1.0 / 4, 1.0 / 4}
  };

  // Create a temporary copy of the image to avoid modifying pixels multiple times
  std::vector<std::vector<Pixel>> temp_image = image;

  // Loop over each pixel in the image
  for (int y = 1; y < image.size() - 1; y++) {
    for (int x = 1; x < image[y].size() - 1; x++) {
    
      // Initialize the sum of colors for the surrounding pixels to zero
      double sum_r = 0;
      double sum_g = 0;
      double sum_b = 0;

      // Loop over each pixel in the kernel
      for (int ky = -1; ky <= 1; ky++) {
        for (int kx = -1; kx <= 1; kx++) {
          // Compute the index of the neighboring pixel
          int ny = y + ky;
          int nx = x + kx;

          // Multiply the color of the neighboring pixel by the corresponding kernel value
          sum_r += kernel[ky + 1][kx + 1] * image[ny][nx].r;
          sum_g += kernel[ky + 1][kx + 1] * image[ny][nx].g;
          sum_b += kernel[ky + 1][kx + 1] * image[ny][nx].b;
        }
      }

      // Set the color of the current pixel to the weighted average of its neighbors
      temp_image[y][x].r = static_cast<unsigned char>(sum_r);
      temp_image[y][x].g = static_cast<unsigned char>(sum_g);
      temp_image[y][x].b = static_cast<unsigned char>(sum_b);
    }
  }

  // Copy the modified pixels back to the original image
  image = temp_image;
}

// Calculate the gradient magnitude of the image using Sobel edge detection
// Calculate the gradient magnitude of the image using Sobel edge detection
void T3(Image& pixels) {
    // Create a temporary copy of the input pixels
    Image tempPixels = pixels;
    
    // Define the Sobel operator kernels
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    bool PREVENT_ERROR = false;
    // Loop over each pixel in the input pixels
    for (int y = 1; y < pixels.size() - 1; y++) {
        for (int x = 1; x < pixels[y].size() - 1; x++) {
            sem_wait(&sema);
            // Calculate the gradient magnitude of the pixel using the Sobel kernels

            if(position[0]>=x+1||position[1]>=y+1) {
            int gx_sum = 0, gy_sum = 0;
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    gx_sum += gx[j + 1][i + 1] * tempPixels[y + j][x + i].r;
                    gy_sum += gy[j + 1][i + 1] * tempPixels[y + j][x + i].r;
                }
            }
            int mag = sqrt(gx_sum * gx_sum + gy_sum * gy_sum);
            
            // Set the RGB values of the pixel to the gradient magnitude
            pixels[y][x].r = mag;
            pixels[y][x].g = mag;
            pixels[y][x].b = mag;
            PREVENT_ERROR = false;
            }
            else {
                PREVENT_ERROR = true;
            }
            sem_post(&sema);
            if(PREVENT_ERROR) x-=1;
        }
        if(PREVENT_ERROR) y-=1;
    }
}

void writePPM(string filename, Image & pixels, int width, int height) {
    // Open the output file
    ofstream fout(filename, ios::out | ios::binary);
    
    // Write the PPM header
    fout << "P6\n" << width << " " << height << "\n" << 255 << "\n";
    
    // Write the pixel data
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fout << pixels[i][j].r << pixels[i][j].g << pixels[i][j].b;
        }
    }
    
    // Close the output file
    fout.close();
}


int main(int argc, char *argv[]) {
    // Check that the input arguments are valid
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <path-to-original-image> <path-to-transformed-image>" << endl;
        return 1;
    }
    char PPV[10];
    int width, height,max;
    key_t key = 0x1234;
    FILE *inputImage = fopen(argv[1], "r");
    fscanf(inputImage, "%s%d%d%d", PPV, &width, &height, &max);
    fclose(inputImage);
    

    // Read the input PPM file
    struct Pixel *sharedImage;
    int shmid = shmget(key, sizeof(struct Pixel) * (height) * width, 0666 | IPC_CREAT);
    sharedImage = (struct Pixel *)shmat(shmid, NULL, 0);
    readPPM(argv[1], key, width, height);

    // Apply the first transformation (e.g. grayscale)
    // T1(pixels);
    
    // // Apply the second transformation (e.g. edge detection)
    // T3(pixels);
    
    // Write the output PPM file
    // writePPM(argv[2], pixels, width, height);
    
    // Exit the program
    return 0;
}

