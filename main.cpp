#include <bits/stdc++.h>
#include <stdlib.h>
#include <dirent.h>
using namespace std;

struct Node{
    char character;
    unsigned int frequency;
    bool is_char;
    struct Node* left;
    struct Node* right;
};
struct compareNodeptr {
    bool operator()(Node* node1, Node* node2) {
        return node1->frequency > node2->frequency;
    }

};

void inorder(Node *node) {
    if(!node)
        return;
    inorder(node->left);
    cout << node->character <<":"<<node->frequency << endl;
    inorder(node->right);
}
enum operation_mode { compress_file, decompress_file,compress_folder , decompress_folder  };
operation_mode mode = compress_file;

int distinct_char_count = 0;
unordered_map<char, pair<int, short> > char_code;

void huffman_codes(Node* root,int code, short height);
void compute_file_frequency(char* fpath, unordered_map<char ,unsigned int>&freq);
void count_occurences(char *buff ,unordered_map<char , unsigned int>&freq);
Node* generate_huffman_tree(unordered_map<char ,unsigned int>&freq);
Node* create_node(char, unsigned int);
void compress(  char* out_path, unordered_map<char , unsigned int>&freq);
void decompress(char* fpath);
int main(int argc ,char **argv) {

    char x = 255;
    while(x) {
        x<<=1;
    }
    char fpath[512];
    if(argc == 1) {// no inputs through command line then scan the inputs
        printf("Choose mode:\n1-Compress File\n2-Decompress File\n3-Compress Folder\n4-Decompress Folder\n");
        int mode_type;
        scanf("%d", &mode_type);
        mode_type--;
        mode = (operation_mode)mode_type;


        printf("Input Target path:\n");
        scanf("%s",fpath);
    }else{
        // -c compress file
        // -d decompress file
        // -f compress folder
        // -j decompress folder
        if(!strcmp(argv[1], "-c") ) {
            mode = compress_file;
        } else if(!strcmp(argv[1], "-d") ) {
            mode = decompress_file;
        } else if(!strcmp(argv[1], "-f") ) {
            mode = compress_folder;
        } else if(!strcmp(argv[1], "-j") ) {
            mode = decompress_folder;
        } else if(!strcmp(argv[1],"-h") ) {
            printf("Available commands\ncompress file:\"-c\"\ndecompress file:\"-d\"decompress file\n" );
            printf("compress folder\"-f\"\ndecompress folder\"-j\"\n");
            exit(1);

        } else {
            printf("Unknown command \"%s\", enter -h to know available commands",argv[1]);
            exit(0);
        }

        if(argc<2 ) {
            printf("Argument missing, must input path");
            exit(0);
        }
        strcpy(fpath, argv[2]);

    }

    unordered_map<char, string> char_to_code;
    //must read_file change it into string buffer

    switch(mode) {
        case compress_file: {
            unordered_map<char ,unsigned int> frequency;
            compute_file_frequency(fpath,frequency);
            Node *root = generate_huffman_tree(frequency);
            inorder(root);
            huffman_codes(root,0,0);

            compress(fpath,frequency);
            break;
        }
        case decompress_file: {
            cout << "YEAJ";
            decompress(fpath);
            break;
        }
        case decompress_folder:
        case compress_folder: {
            DIR *d;
            struct dirent *dir;
            d = opendir(fpath);
            cout<< d << endl;
            if(d) {
                int ct = 0;

                //strcpy(fpath,"C:\\");
                strcat(fpath,"\\");

                cout<< fpath << endl;
                unordered_map<char, unsigned int> freq;
                while ((dir = readdir(d)) != NULL) {

                    if(ct++ <2) continue;
                    //cout << dir->d_name << endl;
                    char dir_str[128];
                    char* full_name=strcat(strcpy(dir_str, fpath),dir->d_name);
                    cout<<full_name <<endl;

                    if(mode == compress_folder) {
                        compute_file_frequency(fpath, freq);
                        compress( full_name, freq);
                    } else {
                        decompress(fpath);
                    }
                }
            closedir(d);
            }
            break;
        }

    }

    return 0;
}


void count_occurences(char *buffer, unordered_map<char, unsigned int>&frequency) {

    int idx = 0;
    while( buffer[idx] != 0 ) {
        char current = buffer[idx++];
        auto check_iter = frequency.find(current);

        if( check_iter == frequency.end()) {
            distinct_char_count++;
            frequency.insert( make_pair( current, 1));
        } else {
            (check_iter->second)++;
        }

    }

}
void compute_file_frequency(char *fpath, unordered_map<char ,unsigned int>&frequency) {

    FILE *file_ptr = fopen(fpath, "r");
    if(file_ptr == NULL) {
        printf("File does not exist!");
        exit(0);
    }
    char line_buffer[1024];
    while(!feof(file_ptr)) {

        fgets(line_buffer, 1024,file_ptr);

        count_occurences(line_buffer, frequency);

    }
    fclose(file_ptr);
    free(file_ptr);

}

Node* create_node(char character, unsigned int frequency) {
    Node* new_node = (Node*) malloc( sizeof( Node));

    new_node->character = character;
    new_node->frequency = frequency;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

Node* generate_huffman_tree(unordered_map<char , unsigned int>&freq ) {

    priority_queue<Node*, vector<Node*>, compareNodeptr> pq;
    int number_of_chars = 0;
    // create a node for each character and insert into min priority queue
    for(auto iter = freq.begin(); iter != freq.end(); ++iter ) {
        Node* char_node = create_node(iter->first, iter->second);
        char_node->is_char = true;
        pq.push(char_node);
        number_of_chars++;
    }
    //huffman compression algorithm
    for(int idx = 0; idx < number_of_chars -1  ; ++idx ) {
        Node* combined_node;
        Node* left_node = pq.top();
        pq.pop();
        Node* right_node = pq.top();
        pq.pop();
        //cout << "left:" << left_node->character << "right:" <<right_node->character << endl;
        unsigned int sum_freq = right_node->frequency + left_node->frequency;
        combined_node = create_node(0,sum_freq);
        combined_node->left = left_node;
        combined_node->right = right_node;

        combined_node->is_char = false;
        //flag to mark this node as a non character node

        pq.push(combined_node);
    }
    Node* root = pq.top();

    pq.pop();
    return root;
}

void compress(char* fpath,unordered_map<char, unsigned int>&freq) {

    FILE* fp = fopen(fpath, "r");
    FILE* fpwr = fopen("C:\\Users\\MarwanSaad\\Desktop\\out.txt", "w");
    fprintf(fpwr,"%d\n",distinct_char_count);
    for(auto it =freq.begin() ; it != freq.end() ; ++it ) {
        fprintf(fpwr, "%c %d\n",it->first, it->second);
    }
    char ch;
    int tot_length = 0;
    int subbyte_len = 0;
    unsigned current = 0;//good algorithm but does it work for 0?
    while(!feof(fp)) {
            ch = fgetc(fp);
            unsigned int code = char_code[ch].first;
            short length = char_code[ch].second;

            tot_length += length;
            subbyte_len += length;
            if(!current){
                current = code;
            }else {
                current = current << length;
                current |= code;
            }
            if( subbyte_len >=8){
                unsigned int byte_copy = (current<<24)>>24;
                fputc(byte_copy, fpwr);
                //fprintf(fpwr,"%c",byte_copy);
                current >>= 8;
                subbyte_len -= 8;
            }
    }

    if(subbyte_len!=0){
        current <<=(8-subbyte_len);
        //fprintf(fpwr,"%c",current);
        fputc(current ,fpwr);
        //must be able to detect the padding in decompression
    }
    fclose(fp);
    free(fp);
    fclose(fpwr);
    free(fpwr);

}
void decompress(char* fpath) {

    FILE* fd = fopen(fpath,"r");
    FILE* fp_wr = fopen("C:\\Users\\MarwanSaad\\Desktop\\decomp.txt","w");
    int chars_count = 0;
    fscanf(fd, "%d\n", &chars_count);
    unordered_map<char, unsigned int> freq;
    for(int ctr = 0 ; ctr< chars_count; ++ctr) {
        char curr_char;
        unsigned int char_freq;
        fscanf(fd,"%c %u\n", &curr_char, &char_freq);
        freq.insert(make_pair(curr_char, char_freq) );
    }
    Node *root = generate_huffman_tree(freq);
    Node *current = root;
    while( !feof(fd)) {
        unsigned char ch = fgetc(fd);
        /*if(!ch && 0) { // to handle reading a 0 character
            int ct = 8;
            while(ct--) {
                //fprintf(fp_wr, "%c",root->left->character);
                fputc(root->left->character, fp_wr);
            }
        }*/
        for(int ct = 0; ct<8; ++ct ) {   // wont work if character is 0
            unsigned int MSB = (ch & (128)) >> 7;
            ch <<= 1;
            current = (!MSB)? current->left: current->right;
            if(current->is_char) {
                //fprintf(fp_wr, "%c", current->character);
                fputc(current->character, fp_wr);

                current = root;
            }

        }

    }
    // detect padding here
    fclose(fd);
    free(fd);
    fclose(fp_wr);
    free(fp_wr);


}

void huffman_codes(Node* root,int code, short height) {
    if(root == NULL)
        return ;

    if(root->is_char) {
        char_code.insert( make_pair(root->character ,make_pair(code, height)) );
        cout<< root->character << ":" << code << ":"<< height<<endl ;
    }
    huffman_codes(root->left , (code<<1), height + 1);
    huffman_codes(root->right, (code<<1)|1, height + 1); //bitwise or
}
