#include "ikiheader.h"

int simpanFile(TextEditor *ed, const char *nama_file) {
	
    if (ed->head == NULL) {
        return 0;
    }
    FILE *file = fopen(nama_file, "w");
    if (file == NULL) {
        return 0;
    }
    
    Node *bantu = ed->head;
    while (bantu != NULL) {
        fprintf(file, "%s\n", bantu->info); 
        bantu = bantu->next; 
    }
    fclose(file);
    return 1; 
}
