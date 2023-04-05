#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y
void show(void *u, int w, int h)
{
	int (*world)[w] = u;
	// printf("\033[%dn", w);
	// printf("\033[%dt", h);
	printf("\e[1;1H\e[2J");
	printf("\033[H");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) printf(world[y][x] ? "\033[104m  \033[m" : "  ");
		printf("\033[E");
	}
	fflush(stdout);
}

void evolve(void *u, int w, int h)
{
	unsigned (*world)[w] = u;
	unsigned new[h][w];
    # pragma omp parallel for collapse(2)
	for (int y = 0; y < h; y++)
        for (int x = 0; x < h; x++) // iterate over every elemnt in matrix
            {
                int n = 0;
                for (int yn = y - 1; yn <= y + 1; yn++)
                    for (int xn = x - 1; xn <= x + 1; xn++) // iterate over neighboring elements
                        if (world[(yn + h) % h][(xn + w) % w])
							// module is used because when y=h then neighbour is yn=0
                            n++; // if neighbour is alive then increase neighbours count

                if (world[y][x]) n--; // if element is alive then decrease neigbours because it was counted before
                new[y][x] = (n == 3 || (n == 2 && world[y][x]));
            }
    # pragma omp parallel for collapse(2)
	for (int y = 0; y < h; y++)
        for (int x = 0; x < h; x++)
            world[y][x] = new[y][x];
}

void life(int w, int h, char* file_input)
{
	unsigned world[h][w];
	if (file_input == NULL){
	# pragma omp parallel for collapse(2)
	for (int y = 0; y < h; y++)
        for (int x = 0; x < h; x++)
			world[y][x] = rand() < RAND_MAX / 10 ? 1 : 0;
	}
	else{
		# pragma omp parallel for collapse(2)
		for (int y = 0; y < h; y++)
			for (int x = 0; x < h; x++)
				world[y][x] = 0;
		FILE *fp;
		fp = fopen(file_input, "r");
		ssize_t read;
		size_t len = 0;
		char * line = NULL;
		int x;
		int y;
		while ((read = getline(&line, &len, fp)) != -1) {
			sscanf(line, "%d %d", &x, &y);
			world[y][x] = 1;
		}
		fclose(fp);

    }
	for (int i = 0; i < 1000; i++) {
		// show(world, w, h);
		evolve(world, w, h);
		// usleep(20000);
	}
}

int main(int c, char **v)
{
	int w = 0, h = 0;
	char *file_input=NULL;

	if (c > 1) w = atoi(v[1]);
	if (c > 2) h = atoi(v[2]);
	if (c > 3) file_input = v[3];
	if (w <= 0) w = 30;
	if (h <= 0) h = 30;
	// life(w, h, file_input);
	FILE *output;
	output = fopen("time_measure.txt", "w");
	fprintf(output, "[");
	for(int i=1; i<=12; i++){
		omp_set_num_threads(i);
		double t1 = omp_get_wtime();
		life(w,h,file_input);
		double t2 = omp_get_wtime();
		fprintf(output, "[%d, %f],\n", i, t2-t1);
		printf("%d %f\n", i, t2-t1);
	}
	fprintf(output, "]");
	fclose(output);
}