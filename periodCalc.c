/*
 * MIT License
 *
 * Copyright (c) 2024 Fabio Ionut Ion and Lorenzo Bussi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int periodNaive(char *s)
{
   int n = strlen(s);
   int satisfies = FALSE;
   int p = 1;
   while (p < n && !satisfies)
   {
      satisfies = TRUE;
      for (int i = 0; i < n - p && satisfies; i++)
      {
         if (s[i] != s[i + p])
         {
            satisfies = FALSE;
            ++p;
         }
      }
   }
   return p;
}

int periodSmart(char *s)
{
   int n = strlen(s) + 1;
   int r[n];
   r[0] = -1;
   r[1] = 0;
   int i = 2;
   int rj = r[i - 1];

   while (i < n)
   {
      if (s[i - 1] == s[rj])
      {
         r[i] = rj + 1;
         rj = r[i];
         i = i + 1;
      }
      else if (rj == 0)
      {
         r[i] = 0;
         rj = r[i];
         i = i + 1;
      }
      else
      {
         rj = r[rj];
      }
   }
   return strlen(s) - r[n - 1];
}

/*
 * To calculate the median in the average case we use QuickSort
 */
void swap(double *a, double *b)
{
   double t = *a;
   *a = *b;
   *b = t;
}

int partition(double array[], int low, int high)
{
   double pivot = array[high];
   int i = (low - 1);
   for (int j = low; j < high; j++)
   {
      if (array[j] <= pivot)
      {
         i++;
         swap(&array[i], &array[j]);
      }
   }
   swap(&array[i + 1], &array[high]);
   return (i + 1);
}

void quickSort(double array[], int low, int high)
{
   if (low < high)
   {
      int pi = partition(array, low, high);
      quickSort(array, low, pi - 1);
      quickSort(array, pi + 1, high);
   }
}

double duration(struct timespec start, struct timespec end)
{
   return end.tv_sec - start.tv_sec + ((end.tv_nsec - start.tv_nsec) / 1000000000.0);
}

double getResolution()
{
   struct timespec start, end;
   clock_gettime(CLOCK_MONOTONIC, &start);
   do
   {
      clock_gettime(CLOCK_MONOTONIC, &end);
   } while (duration(start, end) == 0.0);
   return duration(start, end);
}

/*
 * Calculate the 100 possible string lengths in the range [1000, 500000]
 */
int stringLength(int i)
{
   double b = pow((double)500, (double)1 / 99);
   return 1000 * (pow(b, i));
}

/*
 * for the medium case, using {a,b,c} as alphabet
 */
void createString(char *stringa, int lun)
{
   char alph[3] = {'a', 'b', 'c'};
   int q = rand() % lun;
   char strTmp[lun + 1];
   for (int j = 0; j < lun; j++)
   {
      if (j < q)
      {
         int r = rand() % 2;
         strTmp[j] = alph[r];
      }
      else if (j == q)
      {
         strTmp[j] = alph[2];
      }
      else
      {
         strTmp[j] = strTmp[(j - 1) % q];
      }
   }
   strTmp[lun] = '\0';
   strcpy(stringa, strTmp);
}

/*
 * sample of 20 strings of the same length lun
 * used only for medium case
 */
void generateSample(char **sample, int lun)
{
   for (int i = 0; i < 20; i++)
   {
      sample[i] = malloc(sizeof(char) * (lun + 1));
      createString(sample[i], lun);
   }
}

/*
 * 100*20 strings:
 *  - 100 are the possible lengths of the strings
 *  - 20 are the strings of the same length
 *
 * medium case
 */
char ***generateStr()
{
   char ***strings = malloc(100 * sizeof(char **));
   int length;
   for (int i = 0; i < 100; i++)
   {
      length = stringLength(i);
      strings[i] = malloc(20 * sizeof(char *));
      generateSample(strings[i], length);
   }
   return strings;
}

/*
 * 100 strings of the best case, i.e. "aaa...aa"
 */
char **generateBestString()
{
   char **strings = malloc(100 * sizeof(char *));
   int length;
   for (int i = 0; i < 100; i++)
   {
      length = stringLength(i);
      strings[i] = malloc(sizeof(char) * (length + 1));
      for (int k = 0; k < length; ++k)
      {
         strings[i][k] = 'a';
      }
      strings[i][length] = '\0';
   }
   return strings;
}

/*
 * 100 strings of the worst case, i.e. "aaa...aab"
 */
char **generateWorstString()
{
   char **strings = malloc(100 * sizeof(char *));
   int length;
   for (int i = 0; i < 100; i++)
   {
      length = stringLength(i);
      strings[i] = malloc(sizeof(char) * (length + 1));
      for (int k = 0; k < length; ++k)
      {
         if (k < length - 1)
         {
            strings[i][k] = 'a';
         }
         else
         {
            strings[i][k] = 'b';
         }
      }
      strings[i][length] = '\0';
   }

   return strings;
}

int main(int argc, char **argv)
{
   srand(time(NULL));
   double tmin = (double)(getResolution() * (1000 + 1));
   printf("Tmin: %lf\n", tmin);

   char ***strs = generateStr();
   char **bestStrs = generateBestString();
   char **worstStrs = generateWorstString();

   struct timespec start, end;

   double naiveMedium[100][20], smartMedium[100][20];
   double naiveBest[100], smartBest[100];
   double naiveWorst[100], smartWorst[100];
   int iterations = 0;

   printf("Begin measurements...\n");

   for (int i = 0; i < 100; i++)
   {
      printf("%d\n", i);

      /******** BEST CASE ********/

      clock_gettime(CLOCK_MONOTONIC, &start);
      do
      {
         periodNaive(bestStrs[i]);
         clock_gettime(CLOCK_MONOTONIC, &end);
         ++iterations;
      } while (tmin > duration(start, end));
      naiveBest[i] = duration(start, end) / iterations;

      iterations = 0;
      clock_gettime(CLOCK_MONOTONIC, &start);
      do
      {
         periodSmart(bestStrs[i]);
         clock_gettime(CLOCK_MONOTONIC, &end);
         ++iterations;
      } while (tmin > duration(start, end));
      smartBest[i] = duration(start, end) / iterations;

      /******** WORST CASE ********/

      iterations = 0;
      clock_gettime(CLOCK_MONOTONIC, &start);
      do
      {
         periodNaive(worstStrs[i]);
         clock_gettime(CLOCK_MONOTONIC, &end);
         ++iterations;
      } while (tmin > duration(start, end));
      naiveWorst[i] = duration(start, end) / iterations;

      iterations = 0;
      clock_gettime(CLOCK_MONOTONIC, &start);
      do
      {
         periodSmart(worstStrs[i]);
         clock_gettime(CLOCK_MONOTONIC, &end);
         ++iterations;
      } while (tmin > duration(start, end));
      smartWorst[i] = duration(start, end) / iterations;

      for (int j = 0; j < 20; j++)
      {
         /******** MEDIUM CASE ********/

         iterations = 0;
         clock_gettime(CLOCK_MONOTONIC, &start);
         do
         {
            periodNaive(strs[i][j]);
            clock_gettime(CLOCK_MONOTONIC, &end);
            ++iterations;
         } while (tmin > duration(start, end));
         naiveMedium[i][j] = duration(start, end) / iterations;

         iterations = 0;
         clock_gettime(CLOCK_MONOTONIC, &start);
         do
         {
            periodSmart(strs[i][j]);
            clock_gettime(CLOCK_MONOTONIC, &end);
            ++iterations;
         } while (tmin > duration(start, end));
         smartMedium[i][j] = duration(start, end) / iterations;
      }
   }

   double timenaiveMedium[100], timesmartMedium[100];

   printf("\nmedian calculation...\n");
   for (int i = 0; i < 100; i++)
   {
      quickSort(naiveMedium[i], 0, 19);
      quickSort(smartMedium[i], 0, 19);
      timenaiveMedium[i] = (naiveMedium[i][9] + naiveMedium[i][10]) / 2;
      timesmartMedium[i] = (smartMedium[i][9] + smartMedium[i][10]) / 2;
   }

   printf("generating csv file\n");
   FILE *temp;

   if ((temp = fopen("./times.csv", "w+")) == NULL)
   {
      printf("error");
   }

   fprintf(temp, "strLen;naiveBest;smartBest;naiveMedium;smartMedium;naiveWorst;smartWorst\n");

   for (int i = 0; i < 100; ++i)
   {
      fprintf(temp, "%13d%13f;%13f;%10f;%10f;%13f;%13f\n", stringLength(i),
              naiveBest[i], smartBest[i], timenaiveMedium[i], timesmartMedium[i],
              naiveWorst[i], smartWorst[i]);
   }

   fclose(temp);
   printf("done.\n");

   return 0;
}
