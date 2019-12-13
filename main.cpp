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
enum operation_mode { compress_file, decompress_file,compress_folder , decompress_folder  };
operation_mode mode = compress_file;


char* process_input();
void count_occurences(string buff ,unordered_map<char , unsigned int>&freq);
Node* generate_huffman_tree(unordered_map<char ,unsigned int>&freq);
Node* create_node(char, unsigned int);
void compress( unordered_map<char, string> &char_to_code, char* out_path);
void decompress(char* fpath);
int main(int argc ,char **argv)
{
    char* fpath;
    if(argc == 1) {// no inputs through command line then scan the inputs
        printf("Choose mode:\n1-Compress File\n2-Decompress File\n3-Compress Folder\n4-Decompress Folder\n");
        int mode_type;
        scanf("%d", &mode_type);
        mode_type--;
        mode = (operation_mode)mode_type;

        fpath = (char*)malloc(512 * sizeof(char) );
        printf("Input Target path:\n");
        fgets(fpath,512,stdin);
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
        fpath = argv[2];
    }

    unordered_map<char, string> char_to_code;
    //must read_file change it into string buffer
    switch(mode) {
        case compress_file: {

            compress(char_to_code,fpath);
            break;
        }
        case decompress_file: {
            decompress(fpath);
            break;
        }
        case decompress_folder:
        case compress_folder: {
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    char* full_name=strcat(fpath,dir->d_name);
                    if(mode == compress_folder) {
                        compress(char_to_code, full_name);
                    } else {
                        decompress(fpath);
                    }
                }
            closedir(d);
            }
            break;
        }

    }

    unordered_map<char, unsigned int> freq;
    count_occurences("aaaaaaabdbc ",freq);
    for(auto it = freq.begin(); it != freq.end(); ++it) {
        cout<<it->first<<":"<< it->second<<endl;
    }
    priority_queue<Node*, vector<Node*>, compareNodeptr> pq;
    Node* root = generate_huffman_tree(freq);
    cout<< root->right->character;

    return 0;
}


void count_occurences(string buffer, unordered_map<char, unsigned int>&frequency) {

    int len = buffer.length();
    for(int idx = 0; idx < len; ++idx ){
        char current = buffer[idx];
        auto check_iter = frequency.find(current);

        if( check_iter == frequency.end()) {
            frequency.insert( make_pair( current, 1));
        } else {
            (check_iter->second)++;
        }

    }

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

void compress(unordered_map<char, string> &char_to_code, char* fpath) {



}
void decompress(char* fpath) {


}
