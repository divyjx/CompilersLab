#include<bits/stdc++.h>
#include <iterator>

#define EPSILON "ε" 

using namespace std;

class NodeBlock;

class Node{
public:
    string val;
    bool is_terminal;
    bool is_start;
    Node (){};
    NodeBlock* block;
    vector<string> First;
    vector<string> Follow;
};

class NodeBlock{
public:
    Node* parent;
    vector<Node*> nodes;
    NodeBlock* nextNB;

    NodeBlock& operator=(const NodeBlock& other) {
        if (this != &other) { // Check for self-assignment
            // Copy parent and nextNB pointers
            parent = other.parent;
            nextNB = other.nextNB;

            // Clear existing nodes and perform deep copy of nodes vector
            for (Node* node : nodes) {
                delete node;
            }
            nodes.clear();
            nodes.reserve(other.nodes.size());
            for (Node* node : other.nodes) {
                nodes.push_back(new Node(*node)); // Assuming Node has a copy constructor
            }
        }
        return *this;
    }
};

unordered_map<string, Node*> node_table;
unordered_map<string, Node*> lr_fixed_table;

bool start_set = false;

void add_block(Node *lhs_node);


void parse_rule(string input)
{
    // creates entry in rule_table
    input.erase(remove(input.begin(), input.end(), '\n'), input.end());
    istringstream iss(input);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
    
    if (tokens.size() <= 2) {
        cerr << "Error: Not enough tokens in input: "<< endl;
        exit(0);
    } else if (tokens[1]!="->") {
        cerr << "Error: Invalid Rule format" << endl;
        exit(0);   
    } else if (tokens[tokens.size()-1]=="|" || tokens[2] == "|"){
        cerr << "Error: Invalid Rule format" << endl;
        exit(0);
    }

    string node_name = tokens[0];

    Node* rule_node = node_table[node_name];
    if (!rule_node) {
        rule_node = new Node;
        node_table[node_name] = rule_node;
    }
    rule_node->is_terminal = true;
    rule_node->val = node_name;
    
    if (not start_set) {
        rule_node->is_start = true;
        start_set = false;
    }

    add_block(rule_node);
    for (int  i = 2; i < tokens.size(); i++)
    {   
        string token = tokens[i];
        if (tokens[i]=="|"){
            add_block(rule_node);
            continue;

        } else {
            // add element in curr block
            Node* token_node = node_table[token];
            if (token_node==nullptr) {
                token_node = new Node;
                token_node->val = token;
                node_table[token] = token_node;
            }
            rule_node->block->nodes.push_back(token_node);
        }
    }
    
}

void fix_recursion(){
    vector<pair<string, Node*>> node_vector(node_table.begin(), node_table.end());

    for (auto it = node_vector.rbegin(); it != node_vector.rend(); ++it) {
        auto rule = *it;
        if (rule.second->is_terminal){
            auto temp = rule.second->block;
            bool lr = false;
            while (temp)
            {
                if (temp->nodes[0]->val==rule.first){
                    lr = true;
                    break;
                }
                temp = temp->nextNB;
            }
            if ( not lr) {
                lr_fixed_table[rule.first] = rule.second;
                continue;
            }
            vector<NodeBlock*> alphas;
            vector<NodeBlock*> betas;
            temp = rule.second->block;
            while (temp)
            {
                if (temp->nodes[0]->val==rule.first)
                    alphas.push_back(temp);
                else 
                    betas.push_back(temp);
                temp = temp->nextNB;
            }
            
            if (betas.size() == 0){
                cerr << "Error : cannot convert this rule " << rule.first << " -> " << endl;
                exit(0); 
            } 

            // Alphas 
            Node* new_node = new Node;
            new_node-> is_terminal = true;
            new_node-> val = rule.first+"'";
            new_node->is_start = false;

            Node* ep_node = node_table[EPSILON];

            NodeBlock* ep_block = new NodeBlock;
            ep_block->nextNB = nullptr;
            ep_block->nodes = {ep_node};
            ep_block->parent = new_node;

            new_node->block = ep_block;
            for (auto &block: alphas){
                block->nodes.erase(block->nodes.begin());
                block->nodes.push_back(new_node);
                block->nextNB = new_node->block;
                new_node->block = block;
            }
            
            // Betas
            rule.second->block = nullptr;
            for (auto &&block : betas)
            {
                block->nextNB = rule.second->block;
                block->nodes.push_back(new_node);
                rule.second->block = block;
            }
            // node_table[new_node->val] = new_node;
            lr_fixed_table[new_node->val] = new_node;
            lr_fixed_table[rule.first] = rule.second;
        }
        else {
            lr_fixed_table[rule.first] = rule.second;
        }
    }
}

void add_block(Node *rule_node)
{
    NodeBlock *block = new NodeBlock;
    block->parent = rule_node;
    block->nextNB = rule_node->block;
    rule_node->block = block;
}


void view_rules(bool reverse, unordered_map<string, Node*> &node_table ){
    vector<pair<string, Node*>> node_vector(node_table.begin(), node_table.end());

    // Iterate over the vector in reverse order

    if (reverse)
    for (auto it = node_vector.rbegin(); it != node_vector.rend(); ++it) {
        auto& node = *it;
        if (node.second->is_terminal){
            cout << node.first << " -> ";
            bool first = true;
            NodeBlock* temp = node.second->block;
            while (temp)
            {   
                if (not first)
                    cout << "| ";
                else first = false;
                for(auto &i: temp->nodes)
                    cout << i->val << " ";
                temp = temp->nextNB;
            }
            cout << endl;
        }
    }
    else
    for (auto it = node_vector.begin(); it != node_vector.end(); ++it) {
        auto& node = *it;
        if (node.second->is_terminal){
            cout << node.first << " : \n";
            NodeBlock* temp = node.second->block;
            while (temp)
            {   
                cout << "| ";
                for(auto &i: temp->nodes)
                    cout << i->val << " ";
                cout << endl;
                temp = temp->nextNB;
            }
        }
    } 
}

int main (){
    
    cout << "Enter rules one by one | this symbol for epsilon (ε) | enter 'exit' to exit\n\n";
    string input;
    getline(cin, input);
    while (input != "exit")
    {
        parse_rule(input);
        getline(cin, input);
    }

    cout << endl;
    cout << "LR fixed rules" << endl;
    
    Node* ep_node = node_table[EPSILON];
    if (!ep_node){
        ep_node = new Node;
        ep_node->val = EPSILON;
        ep_node->is_terminal = false;
        lr_fixed_table[EPSILON] = ep_node;
        node_table[EPSILON] = ep_node;
    }

    fix_recursion();
    view_rules(true, lr_fixed_table);
}