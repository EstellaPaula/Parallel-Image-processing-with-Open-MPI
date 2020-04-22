
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int width, height, maxval, nrElem;

void applyFilterP5MltipleProccess(char filter, float* image, float* t, int start, int end){
	int i, j, k;
	k = 0;
	float smooth[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	float blur[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	float sharpen[] = {0, -2, 0, -2, 11, -2, 0, -2, 0};
	float mean[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
	float emboss[] = {0, -1, 0, 0, 0, 0, 0, 1, 0};

	for(i = 0; i < 9; i++){
		smooth[i] /= 9;
		blur[i] /= 16;
		sharpen[i] /= 3;
	}

	if(filter == 'b') {
		for(i = 0; i < 9; i++){
			smooth[i] = blur[i];
		}
	}
	if(filter == 'S') {
		for(i = 0; i < 9; i++){
			smooth[i] = sharpen[i];
		}
	}
	if(filter == 'm') {
		for(i = 0; i < 9; i++){
			smooth[i] = mean[i];
			
		}
	}
	if(filter == 'e') {
		for(i = 0; i < 9; i++){
			smooth[i] = emboss[i];
		}
	}

	//rest of image
	for(i = start; i < end; i++ ){
		for(j = 0; j < width; j++){
			if(i != 0 && i != (height - 1) && j != 0 && j != (width - 1)){
				//printf("normal");
				t[k] =  (smooth[0] * image[(i - 1) * width + (j - 1)] +
								smooth[1] * image[(i - 1) * width + j] +
								smooth[2] * image[(i - 1) * width + (j + 1)] +
								smooth[3] * image[i * width + (j - 1)] +
								smooth[4] * image[i * width + j] +
								smooth[5] * image[i * width + (j + 1)] +
								smooth[6] * image[(i + 1) * width + (j - 1)] +
								smooth[7] * image[(i + 1) * width + j] +
								smooth[8] * image[(i + 1) * width + (j + 1)]);
			} else {
				if(i == 0 && j != 0 && j != (width - 1)){
					//printf("first line");
					t[k] =  (smooth[3] * image[i * width + (j - 1)] +
									smooth[4] * image[i * width + j] +
									smooth[5] * image[i * width + (j + 1)] +
									smooth[6] * image[(i + 1) * width + (j - 1)] +
									smooth[7] * image[(i + 1) * width + j] +
									smooth[8] * image[(i + 1) * width + (j + 1)]);
				}
				if(i == (height - 1) && j != 0 && j != (width - 1)){
					//printf("last line");
					t[k] = (smooth[0] * image[(i - 1) * width + (j - 1)] +
									smooth[1] * image[(i - 1) * width + j] +
									smooth[2] * image[(i - 1) * width + (j + 1)] +
									smooth[3] * image[i * width + (j - 1)] +
									smooth[4] * image[i * width + j] +
									smooth[5] * image[i * width + (j + 1)]);
				}
				if(j == 0 && i != 0 && i != (height - 1)){
					//printf("first column");
					float terr;
					terr = (smooth[1] * image[(i - 1) * width + j] +
									smooth[2] * image[(i - 1) * width + (j + 1)] +
									smooth[4] * image[i * width + j] +
									smooth[5] * image[i * width + (j + 1)] +
									smooth[7] * image[(i + 1) * width + j] +
									smooth[8] * image[(i + 1) * width + (j + 1)]);
					t[k] = terr;
									

				}
				if(j == (width - 1) && i != 0 && i != (height - 1)){
					//printf("last column");
					t[k] = (smooth[0] * image[(i - 1) * width + (j - 1)] +
									smooth[1] * image[(i - 1) * width + j] +
									smooth[3] * image[i * width + (j - 1)] +
									smooth[4] * image[i * width + j] +
									smooth[6] * image[(i + 1) * width + (j - 1)] +
									smooth[7] * image[(i + 1) * width + j]);
				}
				//colt stanga sus
				if(i == 0 && j == 0) {
					//colt stanga sus
					t[k] = (smooth[4] * image[i * width + j] +
									smooth[5] * image[i * width + (j + 1)] +
									smooth[7] * image[(i + 1) * width + j] +
									smooth[8] * image[(i + 1) * width + (j + 1)]);
				}
				//colt stanga jos
				if(i == (height - 1) && j == 0) {
					//colt stanga jos
					t[k] = (smooth[1] * image[(i - 1) * width + j] +
									smooth[2] * image[(i - 1) * width + (j + 1)] +
									smooth[4] * image[i * width + j] +
									smooth[5] * image[i * width + (j + 1)]);
				}
				//colt dreapta sus
				if(i == 0 && j == (width - 1)) {
					//colt dreapta sus
					t[k] = (smooth[3] * image[i * width + (j - 1)] +
									smooth[4] * image[i * width + j] +
									smooth[6] * image[(i + 1) * width + (j - 1)] +
									smooth[7] * image[(i + 1) * width + j]);
				}
				//colt dreapta jos
				if(i == (height - 1) && j == (width - 1)) {
					//"colt dreapta jos
					t[k] = (smooth[0] * image[(i - 1) * width + (j - 1)] +
									smooth[1] * image[(i - 1) * width + j] +
									smooth[3] * image[i * width + (j - 1)] +
									smooth[4] * image[i * width + j]);
				}
			}
			if(t[k] >= 255){
				t[k] = (unsigned char)(255);
			}
			if(t[k] <= 0){
				t[k] = (unsigned char)(0);
			}
			t[k] = (unsigned char)(t[k]);
			
			k++;
		}
	}	
	return;
}

void applyFilterP6MltipleProccess(char filter, float* image, float* t, int start, int end){
	int i, j, k, f;
	k = 0;
	float smooth[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	float blur[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	float sharpen[] = {0, -2, 0, -2, 11, -2, 0, -2, 0};
	float mean[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
	float emboss[] = {0, -1, 0, 0, 0, 0, 0, 1, 0};

	for(i = 0; i < 9; i++){
		smooth[i] /= 9;
		blur[i] /= 16;
		sharpen[i] /= 3;
	}

	if(filter == 'b') {
		for(i = 0; i < 9; i++){
			smooth[i] = blur[i];
		}
	}
	if(filter == 'S') {
		for(i = 0; i < 9; i++){
			smooth[i] = sharpen[i];
		}
	}
	if(filter == 'm') {
		for(i = 0; i < 9; i++){
			smooth[i] = mean[i];
			
		}
	}
	if(filter == 'e') {
		for(i = 0; i < 9; i++){
			smooth[i] = emboss[i];
		}
	} 

	//rest of image
	for(i = start; i < end; i++ ){
		for(j = 0; j < width; j++){
			if(i != 0 && i != (height - 1) && j != 0 && j != (width - 1)){
				t[k] =  (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3] +
								smooth[1] * image[(i - 1) * width * 3 + j * 3] +
								smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3] +
								smooth[3] * image[i * width * 3 + (j - 1) * 3] +
								smooth[4] * image[i * width * 3 + j * 3] +
								smooth[5] * image[i * width * 3 + (j + 1) * 3] +
								smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3] +
								smooth[7] * image[(i + 1) * width * 3 + j * 3] +
								smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3]);
				
				if(t[k] >= 255){
					t[k] = (unsigned char)(255);
				}
				if(t[k] <= 0){
					t[k] = (unsigned char)(0);
				}
				t[k] = (unsigned char)(t[k]);
				k++;
				t[k] =  (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 1] +
								smooth[1] * image[(i - 1) * width * 3 + j * 3 + 1] +
								smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 1] +
								smooth[3] * image[i * width * 3 + (j - 1) * 3 + 1] +
								smooth[4] * image[i * width * 3 + j * 3 + 1] +
								smooth[5] * image[i * width * 3 + (j + 1) * 3 + 1] +
								smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 1] +
								smooth[7] * image[(i + 1) * width * 3 + j * 3 + 1] +
								smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 1]);
				if(t[k] >= 255){
					t[k] = (unsigned char)(255);
				}
				if(t[k] <= 0){
					t[k] = (unsigned char)(0);
				}
				t[k] = (unsigned char)(t[k]);
				k++;
				t[k] =  (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 2] +
								smooth[1] * image[(i - 1) * width * 3 + j * 3 + 2] +
								smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 2] +
								smooth[3] * image[i * width * 3 + (j - 1) * 3 + 2] +
								smooth[4] * image[i * width * 3 + j * 3 + 2] +
								smooth[5] * image[i * width * 3 + (j + 1) * 3 + 2] +
								smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 2] +
								smooth[7] * image[(i + 1) * width * 3 + j * 3 + 2] +
								smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 2]);
				if(t[k] >= 255){
					t[k] = (unsigned char)(255);
				}
				if(t[k] <= 0){
					t[k] = (unsigned char)(0);
				}
				t[k] = (unsigned char)(t[k]);
				k++;
			} else {
				int d;
				if(i == 0 && j != 0 && j != (width - 1)){
					t[k] =  (smooth[3] * image[i * width * 3 + (j - 1) * 3] +
									smooth[4] * image[i * width * 3 + j * 3] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3]);
				
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] =  (smooth[3] * image[i * width * 3 + (j - 1) * 3 + 1] +
									smooth[4] * image[i * width * 3 + j * 3 + 1] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 1] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 1] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 1] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 1]);

					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] =  (smooth[3] * image[i * width * 3 + (j - 1) * 3 + 2] +
									smooth[4] * image[i * width * 3 + j * 3 + 2] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 2] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 2] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 2] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 2]);

					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;
					
					
				}
				if(i == (height - 1) && j != 0 && j != (width - 1)){
					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3] +
									smooth[3] * image[i * width * 3 + (j - 1) * 3] +
									smooth[4] * image[i * width * 3 + j * 3] + 
									smooth[5] * image[i * width * 3 + (j + 1) * 3]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 1] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3 + 1] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 1] +
									smooth[3] * image[i * width * 3 + (j - 1) * 3 + 1] +
									smooth[4] * image[i * width * 3 + j * 3 + 1] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 1]);

					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 2] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3 + 2] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 2] +
									smooth[3] * image[i * width * 3 + (j - 1) * 3 + 2] +
									smooth[4] * image[i * width * 3 + j * 3 + 2] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 2]);

					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;
				}
				if(j == 0 && i != 0 && i != (height - 1)){
					t[k] = (smooth[1] * image[(i - 1) * width * 3 + j * 3] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3] +
									smooth[4] * image[i * width * 3 + j * 3] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3]);

					
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;			

					t[k] = (smooth[1] * image[(i - 1) * width * 3 + j * 3 + 1] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 1] +
									smooth[4] * image[i * width * 3 + j * 3 + 1] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 1] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 1] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 1]);
					
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;			

					t[k] = (smooth[1] * image[(i - 1) * width * 3 + j * 3 + 2] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 2] +
									smooth[4] * image[i * width * 3 + j * 3 + 2] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 2] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 2] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 2]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;			

				}
				if(j == (width - 1) && i != 0 && i != (height - 1)){
					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3] +
									smooth[3] * image[i * width * 3 + (j - 1) * 3] +
									smooth[4] * image[i * width * 3 + j * 3] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	

					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 1] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3 + 1] +
									smooth[3] * image[i * width * 3 + (j - 1) * 3 + 1] +
									smooth[4] * image[i * width * 3 + j * 3 + 1] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 1] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 1]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	

					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 2] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3 + 2] +
									smooth[3] * image[i * width * 3 + (j - 1) * 3 + 2] +
									smooth[4] * image[i * width * 3 + j * 3 + 2] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 2] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 2]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	
				}
				//colt stanga sus
				if(i == 0 && j == 0) {
					t[k] = (smooth[4] * image[i * width * 3 + j * 3] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	

					t[k] = (smooth[4] * image[i * width * 3 + j * 3 + 1] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 1] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 1] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 1]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	

					t[k] = (smooth[4] * image[i * width * 3 + j * 3 + 2] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 2] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 2] +
									smooth[8] * image[(i + 1) * width * 3 + (j + 1) * 3 + 2]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	
				}
				//colt stanga jos
				if(i == (height - 1) && j == 0) {
					t[k] = (smooth[1] * image[(i - 1) * width * 3 + j * 3] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3] +
									smooth[4] * image[i * width * 3 + j * 3] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3]);
									
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	

					t[k] = (smooth[1] * image[(i - 1) * width * 3 + j * 3 + 1] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 1] +
									smooth[4] * image[i * width * 3 + j * 3 + 1] +
									smooth[5] * image[i * width * 3 + (j + 1) * 3 + 1]);

					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	

					t[k] = (smooth[1] * image[(i - 1) * width * 3 + j * 3 + 2] +
									smooth[2] * image[(i - 1) * width * 3 + (j + 1) * 3 + 2] +
									smooth[4] * image[i * width * 3 + j * 3 + 2] +
									smooth[5] * image[i * width  * 3+ (j + 1) * 3 + 2]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;	
				}
				//colt dreapta sus
				if(i == 0 && j == (width - 1)) {
					t[k] = (smooth[3] * image[i * width * 3 + (j - 1) * 3] +
									smooth[4] * image[i * width * 3 + j * 3] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] = (smooth[3] * image[i * width * 3 + (j - 1) * 3 + 1] +
									smooth[4] * image[i * width * 3 + j * 3 + 1] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 1] +
									smooth[7] * image[(i + 1) * width * 3 + j * 3 + 1]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] = (smooth[3] * image[i * width * 3 + (j - 1) * 3 + 2] +
									smooth[4] * image[i * width  * 3+ j * 3 + 2] +
									smooth[6] * image[(i + 1) * width * 3 + (j - 1) * 3 + 2] +
									smooth[7] * image[(i + 1) * width  * 3+ j * 3 + 2]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;
				}
				//colt dreapta jos
				if(i == (height - 1) && j == (width - 1)) {
					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3] +
									smooth[3] * image[i * width * 3 + (j - 1) * 3] +
									smooth[4] * image[i * width * 3 + j * 3]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 1] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3 + 1] +
									smooth[3] * image[i * width  * 3+ (j - 1) * 3 + 1] +
									smooth[4] * image[i * width * 3+ j * 3 + 1]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;

					t[k] = (smooth[0] * image[(i - 1) * width * 3 + (j - 1) * 3 + 2] +
									smooth[1] * image[(i - 1) * width * 3 + j * 3 + 2] +
									smooth[3] * image[i * width * 3+ (j - 1) * 3 + 2] +
									smooth[4] * image[i * width * 3 + j * 3 + 2]);
					if(t[k] >= 255){
						t[k] = (unsigned char)(255);
					}
					if(t[k] <= 0){
						t[k] = (unsigned char)(0);
					}
					t[k] = (unsigned char)(t[k]);
					k++;
				}
			}
		}
	}	
	return;
}

int main(int argc, char** argv) {
    
	MPI_Init(NULL, NULL);
	// Find out rank, size
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	char* type[2];
	char* temp[47];
	char* tempp;
	//receive arguments
	char* fileIn = argv[1];
	char* fileOut = argv[2];

	char filter1[14];
	int h, nrChar;
	nrChar = 0;
	for(h = 3; h < 13; h++){
		if(argv[h] != NULL){
			if(strcmp(argv[h], "smooth") == 0){
				filter1[nrChar] = 's';
			}
			if(strcmp(argv[h], "sharpen") == 0){
				filter1[nrChar] = 'S';
			}
			if(strcmp(argv[h], "blur") == 0){
				filter1[nrChar] = 'b';
			}
			if(strcmp(argv[h], "mean") == 0){
				filter1[nrChar] = 'm';
			}
			if(strcmp(argv[h], "emboss") == 0){
				filter1[nrChar] = 'e';
			}
			nrChar++;
		} else {
			break;
		}
	}

	if(rank == 0){

		int i, j, f, typeCode, z;
		//read -----------------------------------------------
		//open file
		FILE *fptr;
		fptr = fopen(fileIn, "rb");
		fscanf(fptr, "%c", &type[0]);
		fscanf(fptr, "%c", &type[0]);
		type[1] = '\0';
		for(i = 0; i < 46; i++){
			fscanf(fptr, "%c", &temp[i]);
		}

		fscanf(fptr, "%d", &width);
		fscanf(fptr, "%d", &height);
		fscanf(fptr, "%d", &maxval);
		// read new line
		fscanf(fptr, "%c", &temp[i]);

		if(strcmp("5",type) == 0){
			nrElem = width * height;
			typeCode = 5;
		} else {
			nrElem = width * height * 3;
			typeCode = 6;
		}

		float* image = (float*) calloc (nrElem,sizeof(float));
		float* resultImage = (float*) calloc (nrElem,sizeof(float));

		if(typeCode == 5) {
			for(i = 0; i < height; i++ ){
				for(j = 0; j < width; j++){
					image[i * width + j] = (float) getc(fptr);
				}
			}
		} else {
			for(i = 0; i < nrElem; i++ ){
					image[i] = (float) getc(fptr);
			}
		}
		
		fclose(fptr);

		MPI_Bcast(&nrChar, 1, MPI_INT, 0, MPI_COMM_WORLD);
		//---------------compute---------------------------
		for(z = 0; z < nrChar; z++){
			printf("%c", filter1[z]);
			//only one process
			if(size == 1){
				if(typeCode == 5 ){
					applyFilterP5MltipleProccess(filter1[z], image, resultImage, 0, height);
				} else {
					applyFilterP6MltipleProccess(filter1[z], image, resultImage, 0, height);
				}
			} else {
				int c = (int)(height / size);
				int start = rank * c;
				int end = rank * c + c;
				char charToSend = filter1[z];
				
				MPI_Bcast(&typeCode, 1, MPI_INT, 0, MPI_COMM_WORLD);
				MPI_Bcast(&charToSend, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
				MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
				MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
				MPI_Bcast(image, nrElem, MPI_FLOAT, 0, MPI_COMM_WORLD);

				float* t;
				//apply filter on portion
				if(typeCode == 5){
					t = (float*) calloc (c * width,sizeof(float));
					applyFilterP5MltipleProccess(filter1[z], image, t, start, end);
					MPI_Gather(t, c * width, MPI_FLOAT, resultImage, c * width, MPI_FLOAT, 0, MPI_COMM_WORLD);
				} else {
					t = (float*) calloc (c * width * 3,sizeof(float));
					applyFilterP6MltipleProccess(filter1[z], image, t, start, end);
					MPI_Gather(t, c * width * 3, MPI_FLOAT, resultImage, c * width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
				}

				start = (size - 1) * c + c;
				free(t);
				int k;

				if(typeCode == 5){
					t = (float*) calloc (c * width,sizeof(float));
					applyFilterP5MltipleProccess(filter1[z], image, t, start, height);

					k = 0;
					for(i = start; i < height; i++ ){
						for(j = 0; j < width; j++){
							resultImage[i * width + j ] = t[k];
							k++;
						}
					}	
					free(t);
				} else {
					t = (float*) calloc (c * width * 3,sizeof(float));
					applyFilterP6MltipleProccess(filter1[z], image, t, start, height);

					k = 0;
					for(i = start; i < height; i++ ){
						for(j = 0; j < width; j++){
							resultImage[i * width*3 + j*3] = t[k];
							k++;
							resultImage[i * width*3 + j*3+1] = t[k];
							k++;
							resultImage[i * width*3 + j*3+2] = t[k];
							k++;
						}
					}
					free(t);	
				}
			}
			free(image);
			image = resultImage;
			resultImage = (float*) calloc (nrElem,sizeof(float));
		}

		//write-------------------------------------
		//open file
		fptr = NULL;
		fptr = fopen(fileOut, "wb");

		if(strcmp("5",type) == 0){
			fprintf(fptr, "P5\n");
		} else {
			fprintf(fptr, "P6\n");
		}
		fprintf(fptr, "%u %u\n", width, height);
		fprintf(fptr, "%d\n", maxval);

		if(typeCode == 5){
			for(i = 0; i < height; i++ ){
				for(j = 0; j < width; j++){
					fputc((unsigned char) image[i * width + j], fptr);
				}
			}
		} else {
			for(i = 0; i < nrElem; i++){
				fputc((unsigned char) image[i], fptr);
			}
		}
		free(image);
		fclose(fptr);
	} else {
		int i, j, fileType, z;
		char charToSend ;
		
		MPI_Bcast(&nrChar, 1, MPI_INT, 0, MPI_COMM_WORLD);
		for(z = 0; z < nrChar; z++){
			MPI_Bcast(&fileType, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(&charToSend, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
			MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

			int c = (int)(height / size);
			int start = rank * c;
			int end = rank * c + c;
			if(fileType == 6){
				nrElem = height * width * 3;
			} else {
				nrElem = height * width;
			}
			float* image = (float*) calloc(nrElem,sizeof(float));
			float* resultImage = (float*) calloc (nrElem,sizeof(float));

			MPI_Bcast(image, nrElem, MPI_FLOAT, 0, MPI_COMM_WORLD);
			if(fileType == 5){
				nrElem = c * width;
				float* t = (float*) calloc (nrElem,sizeof(float));
				applyFilterP5MltipleProccess(charToSend, image, t, start, end);
				MPI_Gather(t, c * width, MPI_FLOAT, resultImage, c * width, MPI_FLOAT, 0, MPI_COMM_WORLD);
				free(t);
			} else {
				nrElem = c * width * 3;
				float* t = (float*) calloc (nrElem,sizeof(float));
				applyFilterP6MltipleProccess(charToSend, image, t, start, end);
				MPI_Gather(t, c * width * 3, MPI_FLOAT, resultImage, c * width * 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
				free(t);
			}

			free(image);
			free(resultImage);
		}
		
	}
	
	MPI_Finalize();
}
