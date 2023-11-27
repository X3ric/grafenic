#ifndef UTILS_H
#define UTILS_H

    #include <stdarg.h>
    #include <math.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <limits.h>

    #ifndef PI
        #define PI 3.14159265358979323846f
    #endif

    // Debug

        void ErrorCallback(int error, const char* description) {
            fprintf(stderr, "Error: %s\n", description);
        }

        void ClearOutput() {
            printf("\033[H\033[J");
        }
        
        void print(const char* format, ...) {
            va_list args;
            va_start(args, format);
            vprintf(format, args);
            va_end(args);
        }

    // Text manipulation

        const char* text(const char* format, ...) {
            static char buffer[100];
            va_list args;
            va_start(args, format);
            vsnprintf(buffer, sizeof(buffer), format, args);
            va_end(args);
            return buffer;
        }

        int textint(char *str) {
            return atoi(str);
        }

        unsigned int textlength(const char *text) {
            return strlen(text);
        }

        const char *textsubtext(const char *text, int position, int length) {
            static char buffer[1024];
            strncpy(buffer, text + position, length);
            buffer[length] = '\0';
            return buffer;
        }

        char *textreplace(const char *text, const char *replace, const char *by) {
            int len = strlen(text);
            int len_replace = strlen(replace);
            int len_by = strlen(by);
            int new_len = len - len_replace + len_by;
            char *new_text = malloc(new_len + 1);
            if (!new_text) return NULL;
            const char *current = text;
            char *new_current = new_text;
            while (*current) {
                if (strstr(current, replace) == current) {
                    strcpy(new_current, by);
                    current += len_replace;
                    new_current += len_by;
                } else {
                    *new_current++ = *current++;
                }
            }
            *new_current = '\0';
            return new_text;
        }

        char *textinsert(const char *text, const char *insert, int position) {
            int len = strlen(text);
            int len_insert = strlen(insert);
            char *new_text = malloc(len + len_insert + 1);
            if (!new_text) return NULL;
            strncpy(new_text, text, position);
            strcpy(new_text + position, insert);
            strcpy(new_text + position + len_insert, text + position);
            return new_text;
        }

        const char *textjoin(const char **textList, int count, const char *delimiter) {
            static char buffer[1024];
            buffer[0] = '\0';
            for (int i = 0; i < count; ++i) {
                strcat(buffer, textList[i]);
                if (i < (count - 1)) {
                    strcat(buffer, delimiter);
                }
            }
            return buffer;
        }

        const char **textsplit(const char *text, char delimiter, int *count) {
            *count = 1;
            for (const char *p = text; *p; p++) {
                if (*p == delimiter) (*count)++;
            }
            const char **splits = malloc(*count * sizeof(char *));
            const char *start = text;
            int idx = 0;
            for (const char *p = text; *p; p++) {
                if (*p == delimiter) {
                    int len = p - start;
                    splits[idx] = malloc(len + 1);
                    strncpy((char *)splits[idx], start, len);
                    ((char *)splits[idx])[len] = '\0';
                    idx++;
                    start = p + 1;
                }
            }
            int len = text + strlen(text) - start;
            splits[idx] = malloc(len + 1);
            strncpy((char *)splits[idx], start, len);
            ((char *)splits[idx])[len] = '\0';
            return splits;
        }

        void textappend(char *text, const char *append, int *position) {
            strcat(text, append);
            *position += strlen(append);
        }

        int textfindindex(const char *text, const char *find) {
            const char *found = strstr(text, find);
            return found ? found - text : -1;
        }

        const char *textupper(const char *text) {
            static char buffer[1024];
            int i = 0;
            for (; text[i]; i++) {
                buffer[i] = toupper(text[i]);
            }
            buffer[i] = '\0';
            return buffer;
        }

        const char *textlower(const char *text) {
            static char buffer[1024];
            int i = 0;
            for (; text[i]; i++) {
                buffer[i] = tolower(text[i]);
            }
            buffer[i] = '\0';
            return buffer;
        }

    // Random

        void RandomSeed(unsigned int seed)
        {
            srand(seed);
        }

        int RandomValue(int min, int max)
        {
            if (min > max) {
                int tmp = max;
                max = min;
                min = tmp;
            }
            return (rand()%(abs(max-min)+1) + min);
        }
    
    // Smothing

        float Lerp(float start, float end, float t) {
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;
            return start + (end - start) * t;
        }

        float Easing(const char *text, float t) {
            if (strcmp(text, "Linear") == 0) {
                return t;
            } else if (strcmp(text, "SineIn") == 0) {
                return 1.0f - cosf((t * PI) / 2.0f);
            } else if (strcmp(text, "SineOut") == 0) {
                return sinf((t * PI) / 2.0f);
            } else if (strcmp(text, "SineInOut") == 0) {
                return -(cosf(PI * t) - 1.0f) / 2.0f;
            } else if (strcmp(text, "CubicIn") == 0) {
                return t * t * t;
            } else if (strcmp(text, "CubicOut") == 0) {
                float p = t - 1.0f;
                return p * p * p + 1.0f;
            } else if (strcmp(text, "CubicInOut") == 0) {
                if (t < 0.5f)
                    return 4.0f * t * t * t;
                else {
                    float f = ((2.0f * t) - 2.0f);
                    return 0.5f * f * f * f + 1.0f;
                }
            } else {
                return t;
            }
        }

    // Time

        double GetTime() {
            return glfwGetTime();
        }

        void SetTime(double time) {
            glfwSetTime(time);
        }

        bool Wait(double delaySeconds) {
            static double startTime = 0;
            static bool isWaiting = false;
            if (!isWaiting) {
                startTime = glfwGetTime();
                isWaiting = true;
                return false;
            }
            if (glfwGetTime() - startTime > delaySeconds) {
                isWaiting = false;
                return true;
            }
            return false;
        }
    
    // Ratio resize

        int Scaling(int fontsize) {
            float widthScale = (float)SCREEN_WIDTH / WIDTH;
            float heightScale = (float)SCREEN_HEIGHT / HEIGHT;
            float scale = fmin(widthScale, heightScale);
            int scaledFontSize = (int)(fontsize * scale);
            return (fontsize > 0) ? fmax(scaledFontSize, 1) : 0;
        }

    // Saving

        #define FILE_LENGTH 100
        #define DATA_FILE "res/data.txt"

        typedef struct FileValue {
            char key[FILE_LENGTH];
            char value[FILE_LENGTH];
            struct FileValue *next;
        } FileValue;

        FileValue *head = NULL;

        bool FileExists(const char* filename) {
            FILE* file = fopen(filename, "r");
            if (file) {
                fclose(file);
                return true;
            }
            return false;
        }

        void FileWrite() {
            FILE *file = fopen(DATA_FILE, "w");
            if (file == NULL) {
                perror("Error opening file for writing");
                return;
            }
            FileValue *current = head;
            while (current != NULL) {
                fprintf(file, "%s,%s\n", current->key, current->value);
                current = current->next;
            }
            fclose(file);
        }

        void FileSave(const char *key, const char *value) {
            FileValue **current = &head;
            while (*current != NULL && strcmp((*current)->key, key) != 0) {
                current = &(*current)->next;
            }
            if (*current == NULL) {
                *current = (FileValue *)malloc(sizeof(FileValue));
                strcpy((*current)->key, key);
                strcpy((*current)->value, value);
                (*current)->next = NULL;
            } else {
                strcpy((*current)->value, value);
            }
            FileWrite();
        }

        void FileLoad(const char *key, char *value) {
            FileValue *current = head;
            while (current != NULL) {
                if (strcmp(current->key, key) == 0) {
                    strcpy(value, current->value);
                    return;
                }
                current = current->next;
            }
            strcpy(value, "");
        }

        void FileClear() {
            FileValue *current = head;
            FileValue *next;
            while (current != NULL) {
                next = current->next;
                free(current);
                current = next;
            }
            head = NULL;
            if (remove(DATA_FILE) != 0) {
                perror("Error deleting file");
            } else {
                printf("File successfully deleted\n");
            }
        }

        void FileLoadAll() {
            char key[FILE_LENGTH], value[FILE_LENGTH];
            FILE *file = fopen(DATA_FILE, "r");
            if (file == NULL) {
                perror("Error opening file for reading");
                return;
            }
            while (fscanf(file, "%99[^,],%99[^\n]\n", key, value) == 2) {
                FileSave(key, value);
            }
            fclose(file);
        }

        time_t GetFileModificationTime(const char* filePath) {
            struct stat attrib;
            stat(filePath, &attrib);
            return attrib.st_mtime;
        }

    // OpenGL utils

        void glTexOpt(GLint filter,GLint warp){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        } 

#endif // UTILS_H
