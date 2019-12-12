#include <bits/stdc++.h>
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


string path = "";
void count_occurences(string buff ,unordered_map<char , unsigned int>&freq);
Node* generate_huffman_tree(unordered_map<char ,unsigned int>&freq);
Node* create_node(char, unsigned int);
int main()
{

    unordered_map<char, unsigned int> freq;
    count_occurences("aabbc ",freq);
    for(auto it = freq.begin(); it != freq.end(); ++it) {
        cout<<it->first<<":"<< it->second<<endl;
    }
    priority_queue<Node*, vector<Node*>, compareNodeptr> pq;
    Node* root = generate_huffman_tree(freq);
    cout<< root->is_char;

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
    for(auto iter = freq.begin(); iter != freq.end(); ++iter ) {
        Node* char_node = create_node(iter->first, iter->second);
        char_node->is_char = true;
        pq.push(char_node);
        number_of_chars++;
    }
    cout<< number_of_chars<<" heh";
    for(int idx = 0; idx < number_of_chars -1  ; ++idx ) {
        Node* combined_node;
        Node* left_node = pq.top();
        pq.pop();
        Node* right_node = pq.top();
        pq.pop();
        unsigned int sum_freq = right_node->frequency + left_node->frequency;
        combined_node = create_node(0,sum_freq);
        combined_node->left = left_node;
        combined_node->right = right_node;
        combined_node->is_char = false;
        pq.push(combined_node);
    }
    Node* root = pq.top();
    pq.pop();
    return root;
}
