/*Dobrinoiu David 313CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Definim structurile pentru pixel si nodul arborelui de compresie
typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;

typedef struct Node
{
    Pixel color;
    int size;
    int level;
    struct Node *nw;
    struct Node *ne;
    struct Node *sw;
    struct Node *se;
} Node;

// Functie pentru calculul culorii medii a unui bloc
Pixel calculate_average_color(Pixel **grid, int x, int y, int size)
{
    unsigned long long average_color_red = 0, average_color_green = 0, average_color_blue = 0;
    for (int i = x; i < x + size; i++)
    {
        for (int j = y; j < y + size; j++)
        {
            average_color_red += grid[i][j].red;
            average_color_green += grid[i][j].green;
            average_color_blue += grid[i][j].blue;
        }
    }
    average_color_red = average_color_red / (size * size);
    average_color_green = average_color_green / (size * size);
    average_color_blue = average_color_blue / (size * size);
    Pixel average_color = {average_color_red, average_color_green, average_color_blue};
    return average_color;
}

// Functie pentru calculul scorului de similaritate a unui bloc
int calculate_similarity_score(Pixel **grid, Pixel color, int x, int y, int size)
{
    unsigned long long score = 0;

    for (int i = x; i < x + size; i++)
    {
        for (int j = y; j < y + size; j++)
        {
            score += (1ULL * color.red - grid[i][j].red) * (1ULL * color.red - grid[i][j].red);
            score += (1ULL * color.green - grid[i][j].green) * (1ULL * color.green - grid[i][j].green);
            score += (1ULL * color.blue - grid[i][j].blue) * (1ULL * color.blue - grid[i][j].blue);
        }
    }
    return score / (3ULL * size * size);
}

// Functie pentru construirea arborelui de compresie
Node *build_quadtree(Node *node, Pixel **grid, int x, int y, int size, int threshold)
{
    node = (Node *)malloc(sizeof(Node));

    // Calculam culorea medie a blocului si scorul de similaritate
    Pixel average_color = calculate_average_color(grid, x, y, size);
    int similarity_score = calculate_similarity_score(grid, average_color, x, y, size);

    // Daca blocul trebuie sa fie divizat in sub-blocuri
    if (size > 1 && similarity_score > threshold)
    {
        node->color = average_color;
        node->nw = build_quadtree(node, grid, x, y, size / 2, threshold);
        node->ne = build_quadtree(node, grid, x, y + size / 2, size / 2, threshold);
        node->sw = build_quadtree(node, grid, x + size / 2, y, size / 2, threshold);
        node->se = build_quadtree(node, grid, x + size / 2, y + size / 2, size / 2, threshold);
        node->level = node->nw->level + 1;
    }
    // Daca blocul nu mai poate fi divizat, il consideram fiind o frunza si setam valorile corespunzatoare
    else
    {
        node->size = size;
        node->color = average_color;
        node->nw = NULL;
        node->ne = NULL;
        node->sw = NULL;
        node->se = NULL;
        node->level = 0;
    }

    return node;
}

int maxi(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

// Functie pentru numararea nivelurilor din arborele cuaternar
int count_levels(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    else
    {
        int nw_level = count_levels(root->nw);
        int ne_level = count_levels(root->ne);
        int sw_level = count_levels(root->sw);
        int se_level = count_levels(root->se);
        int max_child_level = maxi(maxi(nw_level, ne_level), maxi(sw_level, se_level));
        return max_child_level + 1;
    }
}

// Functie pentru numararea blocurilor cu scorul de similaritate mai mic sau egal cu un factor dat
void count_blocks_below_threshold(Node *root, int *count)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        // Verificam daca scorul de similaritate este mai mic sau egal cu factorul dat
        if (root->nw == NULL && root->ne == NULL && root->sw == NULL && root->se == NULL)
        {
            (*count)++;
        }

        // Apelam recursiv functia pentru fiecare copil al nodului
        count_blocks_below_threshold(root->nw, count);
        count_blocks_below_threshold(root->ne, count);
        count_blocks_below_threshold(root->sw, count);
        count_blocks_below_threshold(root->se, count);
    }
}

int find_largest_square(Node *node)
{
    // Daca nodul este frunza, returnam dimensiunea laturii patratului
    if (node->nw == NULL && node->ne == NULL && node->sw == NULL && node->se == NULL)
    {
        return node->size;
    }

    // Determinam dimensiunea celui mai mic patrat nedivizat pentru fiecare copil
    int nw_size = find_largest_square(node->nw);
    int ne_size = find_largest_square(node->ne);
    int sw_size = find_largest_square(node->sw);
    int se_size = find_largest_square(node->se);

    // Returnam dimensiunea maxima dintre cele determinate anterior
    return maxi(maxi(nw_size, ne_size), maxi(sw_size, se_size));
}

int factr(char *argv)
{
    int factor = 0;
    for (int i = 0; argv[i]; i++)
        factor = factor * 10 + argv[i] - '0';
    return factor;
}

void printLevelK(struct Node *root, int k, FILE **gp)
{
    if (root == NULL)
    {
        return;
    }
    if (k == 0)
    {
        unsigned char T = 0;
        if (root->nw == NULL && root->ne == NULL && root->sw == NULL && root->se == NULL)
        {
            unsigned char T = 1;
            fwrite(&T, sizeof(unsigned char), 1, *gp);
            fwrite(&root->color.red, sizeof(unsigned char), 1, *gp);
            fwrite(&root->color.green, sizeof(unsigned char), 1, *gp);
            fwrite(&root->color.blue, sizeof(unsigned char), 1, *gp);
        }
        else
            fwrite(&T, sizeof(unsigned char), 1, *gp);
        return;
    }
    printLevelK(root->nw, k - 1, gp);
    printLevelK(root->ne, k - 1, gp);
    printLevelK(root->se, k - 1, gp);
    printLevelK(root->sw, k - 1, gp);
}

void deallocate_quadtree(Node **node)
{
    if (*node == NULL)
    {
        return;
    }
    deallocate_quadtree(&(*node)->nw);
    deallocate_quadtree(&(*node)->ne);
    deallocate_quadtree(&(*node)->sw);
    deallocate_quadtree(&(*node)->se);
    free(*node);
}

int main(int argc, char **argv)
{
    int n, m;
    Node *root = NULL;
    if (strcmp(argv[1], "-c1") == 0)
    {
        // Vom rezolva cerinta 1
        int factor = factr(argv[2]);
        char v[100], c[100];
        FILE *fp = fopen(argv[3], "rb");
        fscanf(fp, "%s%d%d\n%s", c, &m, &n, v);
        fgetc(fp);
        // Alocam memorie pentru matricea de pixeli
        Pixel **grid = (Pixel **)malloc(n * sizeof(Pixel *));
        for (int i = 0; i < n; i++)
            grid[i] = (Pixel *)malloc(n * sizeof(Pixel));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                fread(&grid[i][j], sizeof(Pixel), 1, fp);
        root = build_quadtree(root, grid, 0, 0, n, factor);
        FILE *gp = fopen(argv[4], "w");

        int count = 0;
        count_blocks_below_threshold(root, &count);
        fprintf(gp, "%d\n%d\n%d\n", count_levels(root), count, find_largest_square(root));
        deallocate_quadtree(&root);
        for (int i = 0; i < n; i++)
            free(grid[i]);
        free(grid);
        fclose(fp);
        fclose(gp);
    }
    else if (strcmp(argv[1], "-c2") == 0)
    {
        // Vom rezolva cerinta 2
        int factor = factr(argv[2]);
        char v[100], c[100];
        FILE *fp = fopen(argv[3], "rb");
        fscanf(fp, "%s%d%d\n%s", c, &m, &n, v);
        fgetc(fp);
        // Alocam memorie pentru matricea de pixeli
        Pixel **grid = (Pixel **)malloc(n * sizeof(Pixel *));
        for (int i = 0; i < n; i++)
            grid[i] = (Pixel *)malloc(n * sizeof(Pixel));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                fread(&grid[i][j], sizeof(Pixel), 1, fp);
        root = build_quadtree(root, grid, 0, 0, n, factor);
        FILE *gp = fopen(argv[4], "wb");
        int i, nr_levels = count_levels(root);
        fwrite(&n, sizeof(int), 1, gp);
        for (i = 0; i < nr_levels; i++)
            printLevelK(root, i, &gp);
        deallocate_quadtree(&root);
        for (int i = 0; i < n; i++)
            free(grid[i]);
        free(grid);
        fclose(fp);
        fclose(gp);
    }
    return 0;
}
