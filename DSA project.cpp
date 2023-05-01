//DSA project           Date: 2nd January 2022
//Submitted by: Hareem Bilal Malik | 344043 and Ali Abdullah | 335428
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

class song {
public:
    //characteristics of a song
    string name;
    string id;
    string album;
    string album_id;
    string artists;
    string artist_ids;
    string track_number;
    string disc_number;
    string explicit1;
    string danceability;
    string energy;
    string key;
    string loudness;
    string mode;
    string speechines;
    string acousticness;
    string instrument;
    string liveness;
    string valence;
    string tempo;
    string duration_ms;
    string time_signature;
    string year;
    string release_date;
};
string removespecialchar(string);//removes special characters like symbols, accent marks, and punctuation marks from a string

//Trie implementation
class T_node {//node class
public:
    string data;
    T_node* children[26];
    bool endofword; //indicates the end of song name
    int key;
};

class trie {
public:
    T_node* root = new T_node();
    T_node* loc = new T_node();
    trie() {//default constructor; initialises trie to 0
        root->endofword = false;
        root->key = NULL;
        for (int i = 0; i < 26; i++)
            root->children[i] = NULL;
    }
    bool is_empty(); //returns true if the trie is empty
    int search(string); //searches a song name and returns its key
    void insert(string); //inserts a song into the trie
    //makes the song key by converting each character in song name to its ASCII equivalent and summing the values up 
    int makekey(string);
};

bool trie::is_empty() {
    for (int i = 0; i < 26; i++) {
        if (root->children[i] != NULL)
            return false;
    }
    return true;
}
void trie::insert(string val) {
    loc = root;
    for (int i = 0; i < val.length(); i++) {
        int index = val[i] - 'A';
        if (loc->children[index] == NULL)
            loc->children[index] = new T_node();
        loc = loc->children[index];
    }
    //once all the characters of song name have been inserted, endofword is set to true and key is made
    loc->endofword = true;
    loc->key = makekey(val);
}

int trie::search(string val) {
    if (is_empty()) {
        return 0;
    }
    loc = root;
    for (int i = 0; i < val.length(); i++) {
        int index = val[i] - 'A';
        if (loc->children[index] == NULL) {

            return NULL;
        }
        loc = loc->children[index];
    }
    if (loc->endofword)
        return loc->key;//key is returned when endofword becomes true
}

int trie::makekey(string val) {
    int sum = 0;
    for (int i = 0; i < val.length(); i++) {
        sum += int(val[i]);//summing up the ASCII values of each character in song name, and returning the sum as key
    }
    return sum;
}


//AVL tree implementation
class AVL_node
{
public:
    song data;
    int height;
    AVL_node* R_child;
    AVL_node* L_child;
};

class AVL
{
public:
    AVL_node* root, * loc, * ploc;
    AVL()//defaut constructor; makes empty tree
    {
        root = NULL;
        loc = NULL;
        ploc = NULL;
    }
    bool is_empty(); //returns true if the tree is empty; false otherwise
    AVL_node* search(string);//searches a value in the AVL tree
    AVL_node* newNode(song);//makes a new node
    AVL_node* insertwithoutduplication(AVL_node*, song);//calls the search() function to insert a new value in the AVL tree; does not allow duplication
    //traversal methods
    void preorder(AVL_node*);
    void postorder(AVL_node*);
    void inorder(AVL_node*);
    int calc_height(AVL_node*);//computes the height of the AVL tree
    AVL_node* deletenode(AVL_node*, string);//deletes a node of the AVL tree
    void destroytree(AVL_node*);//deletes all nodes of the AVL tree leaving it empty
    int balancevec(AVL_node*);//calculates the balance factor
    //rotation operations
    AVL_node* rightrot(AVL_node*);
    AVL_node* leftrot(AVL_node*);
    AVL_node* minvalnode(AVL_node*);//returns the smallest value of the AVL tree
};

void song_attributes(AVL_node* node);

bool AVL::is_empty()
{
    return root == NULL;
}

int max(int a, int b) //returns the larger of 2 values
{
    return (a > b) ? a : b;
}

AVL_node* AVL::minvalnode(AVL_node* node)
{
    AVL_node* current = node;

    // looping down to find the leftmost leaf
    while (current->L_child != NULL)
        current = current->L_child;

    return current;
}

void AVL::preorder(AVL_node* node)//Depth first order: Visiting the root, then the left subtree followed by the right subtree
{
    if (is_empty())
        return;
    if (node != NULL)
    {
        song_attributes(node);
        cout << "\n\n--->\n\n";
        preorder(node->L_child);
        preorder(node->R_child);
    }

}

void AVL::inorder(AVL_node* node)//Traversing a non-empty AVL tree in symmetric order
{
    if (is_empty())
        return;
    if (node != NULL)
    {
        inorder(node->L_child);
        cout << "\x1b[35m" << node->data.name<<", by "<< node->data.artists << "\x1b[32m ~ \x1b[35m" << node->data.id;
        cout << "\n\n\x1b[37m--->\n\n";
        inorder(node->R_child);
    }
}

void AVL::postorder(AVL_node* node)//Traversing the left subtree, then the right subtree. Visiting the root last.
{
    if (is_empty())
        return;
    if (node != NULL)
    {
        postorder(node->L_child);
        postorder(node->R_child);
        song_attributes(node);
        cout << "\n\n--->\n\n";
    }
}

AVL_node* AVL::search(string value) {

    if (is_empty())
        return 0;
    loc = root;
    ploc = NULL;
    while (loc != NULL) {//If the searched value is found, loc should points to the node in which the searched value is found, and ploc to its parent node
        if (value > loc->data.id) {
            ploc = loc;
            loc = loc->R_child;
        }
        else if (value < loc->data.id) {
            ploc = loc;
            loc = loc->L_child;
        }
        else
            return loc;
    }
    return loc;
}

AVL_node* AVL::newNode(song val)
{
    AVL_node* node = new AVL_node();
    node->data = val;
    node->L_child = NULL;
    node->R_child = NULL;
    node->height = 0; // new node is initially added at leaf
    return(node);
}

int AVL::balancevec(AVL_node* nn)
{
    if (nn == NULL)
        return 0;
    return calc_height(nn->L_child) - calc_height(nn->R_child);
}

AVL_node* AVL::rightrot(AVL_node* x)
{
    AVL_node* y = x->L_child;
    AVL_node* z = y->R_child;

    // Perform right rotation
    x->L_child = z;
    y->R_child = x;

    // Update heights
    y->height = calc_height(y);
    x->height = calc_height(x);

    // Return new root
    return y;
}

AVL_node* AVL::leftrot(AVL_node* x)
{
    AVL_node* y = x->R_child;
    AVL_node* z = y->L_child;

    // Perform left rotation
    x->R_child = z;
    y->L_child = x;

    // Update heights
    x->height = calc_height(x);
    y->height = calc_height(y);

    // Return new root
    return y;
}

AVL_node* AVL::insertwithoutduplication(AVL_node* curr, song val)
{
    if (is_empty()) {  // inserting the first node, if root is NULL
        root = newNode(val);
        return root;
    }

    if (!curr) {
        return newNode(val);
    };

    if (val.id < curr->data.id)
        curr->L_child = insertwithoutduplication(curr->L_child, val);
    else if (val.id > curr->data.id)
        curr->R_child = insertwithoutduplication(curr->R_child, val);
    else
        //If the searched value already exists in the tree, its duplicate should not be inserted
        cout << "Value already exists. Duplication not allowed." << val.name << endl;
    return curr;

    // Update height
    curr->height = calc_height(curr);

    // Left Left case
    if (balancevec(curr) > 1 && val.id < curr->L_child->data.id)
        return rightrot(curr);

    // Right Right Case
    if (balancevec(curr) < -1 && val.id > curr->R_child->data.id)
        return leftrot(curr);

    // Left Right Case
    if (balancevec(curr) > 1 && val.id > curr->L_child->data.id)
    {
        curr->L_child = leftrot(curr->L_child);
        return rightrot(curr);
    }

    // Right Left Case
    if (balancevec(curr) < -1 && val.id < curr->R_child->data.id)
    {
        curr->R_child = rightrot(curr->R_child);
        return leftrot(curr);
    }
    return curr;

}

int AVL::calc_height(AVL_node* curr)
{
    if (curr == NULL)
        return -1;
    else
        return 1 + max(calc_height(curr->L_child), calc_height(curr->R_child));
}

AVL_node* AVL::deletenode(AVL_node* root, string key)
{

    //if the tree is empty then return
    if (root == NULL)
        return root;

    // If the key to be deleted is smallerthan the root's key, then it lies in left subtree
    if (key < root->data.id)
        root->L_child = deletenode(root->L_child, key);

    // If the key to be deleted is greater than the root's key, then it lies in right subtree
    else if (key > root->data.id)
        root->R_child = deletenode(root->R_child, key);

    // if key is same as root's key, then this is the node to be deleted
    else
    {
        // node with only one child or no child
        if ((root->L_child == NULL) || (root->R_child == NULL))
        {
            AVL_node* temp = root->L_child ? root->L_child : root->R_child;

            if (temp == NULL) // No child case
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                *root = *temp; // Copying the contents of the non-empty child
            free(temp);
        }
        else //node with two children: Getting the inorder successor (smallest in the right subtree)
        {
            AVL_node* temp = minvalnode(root->R_child);

            // Copying the inorder successor's data to this node
            root->data = temp->data;

            // Deleting the inorder successor
            root->R_child = deletenode(root->R_child, temp->data.id);
        }
    }

    //If the tree had only one node then return
    if (root == NULL)
        return root;

    //Updating height of the current node
    root->height = 1 + max(calc_height(root->L_child),
        calc_height(root->R_child));

    //Getting the balance factor of the current node (to check whether this node became unbalanced)

    //If the current node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balancevec(root) > 1 && balancevec(root->L_child) >= 0)
        return rightrot(root);

    // Left Right Case
    if (balancevec(root) > 1 && balancevec(root->L_child) < 0)
    {
        root->L_child = leftrot(root->L_child);
        return rightrot(root);
    }

    // Right Right Case
    if (balancevec(root) < -1 && balancevec(root->R_child) <= 0)
        return leftrot(root);

    // Right Left Case
    if (balancevec(root) < -1 && balancevec(root->R_child) > 0)
    {
        root->R_child = rightrot(root->R_child);
        return leftrot(root);
    }

    return root;
}

void AVL::destroytree(AVL_node* curr)
{
    if (!is_empty())
    {
        if (curr != NULL)
        {
            destroytree(curr->L_child);
            destroytree(curr->R_child);
            delete curr;

        }
    }
}

//Hashtable implementation
class hashtable {
public:
    int hashtable_size = 10000;
    AVL* data = new AVL[hashtable_size];
    int noofinsertions=0;
    void insert(int, song); //inserting value into AVL tree at a particular index of hashtable; a seperate chaining method
    int hashfunc(int); //hash function 1
    void searchvalintree(int, string); //returns a searched node of AVL tree at a particular index of hashtable
    void deletenode(int, song); //deletes value from AVL tree at a particular index of hashtable
    void displaytree(int); //displays the AVL tree at a particular index of hashtable
    void dynamicexpansion();
    void dynamicreduction();

};

void hashtable::insert(int key, song value) {
    int index = hashfunc(key);
    //cout << "\x1B[34m" << value.name << "\x1B[32m by \x1B[33m" << value.artists << "\x1B[32m ~ \x1b[35m" << value.id <<"  "<<index<< "\n";
    data[index].insertwithoutduplication(data[index].root, value);
    noofinsertions++;
    dynamicexpansion();
}

void hashtable::dynamicexpansion() {
    int loadfactor = noofinsertions / hashtable_size;
    if (loadfactor > 20)
        hashtable_size= hashtable_size*2;
}

void hashtable::dynamicreduction() {
    int loadfactor = noofinsertions / hashtable_size;
    if (loadfactor < 5)
        hashtable_size = hashtable_size / 2;
}

void hashtable::deletenode(int key, song value) {
    int index = hashfunc(key);
    //cout << "\x1B[32mDeleting value \x1B[34m" << value.name << "\x1B[32m from avl tree at index \x1B[33m" << index << "\x1B[32m:\n";
    data[index].deletenode(data[index].root, value.id);
    noofinsertions--;
    dynamicreduction();
}

void hashtable::displaytree(int key) {
    int index = hashfunc(key);
    //cout << "\x1B[32mDisplaying the avl tree at index \x1B[33m" << index << "\x1B[32m in inorder sequence:\n";
    data[index].inorder(data[index].root);
}

int hashtable::hashfunc(int key) {
    return key % hashtable_size;
}

void hashtable::searchvalintree(int key, string value) {
    int index = hashfunc(key);
    //cout << "\n\x1B[36m" << value << " is located in a tree at hashtable index " << index << ".\n\n";
    AVL_node* found = data[index].search(value);
    if (found != NULL)
        song_attributes(found);
    else
        cout << "\x1b[31mSong not found!\n";
}


int main()
{
    AVL a;
    song s;
    hashtable h;
    trie t;

    ifstream fin;
    fin.open("tracks_features.csv");//opening the CSV file
    if (!fin.is_open())//if the file fails to open, displaying error msg
        std::cout << "Error in opening file" << endl;

    string line;
    int i = 0;
    while (i < 1000) {//For the first 1000 songs stored in the file
        getline(fin, line);
        /*stringstream associates the string object, line with the stream ss allowing the string to be read
        as if it were a stream (like cin).*/
        stringstream ss(line);
        /*The getline() function extracts characters from the input stream and appends it to the string object
        until the delimiting character ',' is encountered*/
        getline(ss, s.id, ',');
        getline(ss, s.name, ',');
        getline(ss, s.album, ',');
        getline(ss, s.album_id, ',');
        getline(ss, s.artists, ',');
        getline(ss, s.artist_ids, ',');
        getline(ss, s.track_number, ',');
        getline(ss, s.disc_number, ',');
        getline(ss, s.explicit1, ',');
        getline(ss, s.danceability, ',');
        getline(ss, s.energy, ',');
        getline(ss, s.key, ',');
        getline(ss, s.loudness, ',');
        getline(ss, s.mode, ',');
        getline(ss, s.speechines, ',');
        getline(ss, s.acousticness, ',');
        getline(ss, s.instrument, ',');
        getline(ss, s.liveness, ',');
        getline(ss, s.valence, ',');
        getline(ss, s.tempo, ',');
        getline(ss, s.duration_ms, ',');
        getline(ss, s.time_signature, ',');
        getline(ss, s.year, ',');
        getline(ss, s.release_date, ',');

        s.name = removespecialchar(s.name);
        s.id = removespecialchar(s.id);
        s.artists = removespecialchar(s.artists);
        s.artist_ids = removespecialchar(s.artist_ids);
        std::transform(s.name.begin(), s.name.end(), s.name.begin(), ::toupper);//converting song name to uppercase before insertion
        std::transform(s.id.begin(), s.id.end(), s.id.begin(), ::toupper); //converting song ID to uppercase before insertion

        //a.root = a.insertwithoutduplication(a.root, s);
        t.insert(s.name);//storing the song names in a trie
        h.insert(t.search(s.name), s);//storing song details in avl trees at particular indexes  
        i++;
    }
    //h.displaytree(t.search(s.name));
    fin.close();
    bool go = true;
    while (go) {
        std::cout << "\x1b[36m\nEnter a song to see its details:\x1b[32m\n";
        string input1, input2;
        cout << "\x1b[34mSong: \x1b[32m";
        getline(cin, input1);//getting song name input
        //special characters removed from input
        input1 = removespecialchar(input1);
        //Both inputs are converted to uppercase before searching
        std::transform(input1.begin(), input1.end(), input1.begin(), ::toupper);
        std::transform(input2.begin(), input2.end(), input2.begin(), ::toupper);
        //The trie search function returns the key of the searched song (using the song name).
        //The hash search function uses this key to compute the hashtable index, using hashfunc.
        /*The AVL tree search function, a part of the hash search function, uses the song ID
        to find the location of the song in the tree at that index*/
        if (t.search(input1) == NULL)
            cout << "\x1b[31mSong not found!\n";
        else {
            cout << "\n\x1b[34mThe following results showed up; please select ID from below:\n" << endl;
            h.displaytree(t.search(input1));
            cout << "\x1b[34mID: \x1b[32m";
            getline(cin, input2);//getting song ID input
            input2 = removespecialchar(input2);
            h.searchvalintree(t.search(input1), input2);
        }
        std::cout << "\x1b[36mEnter 1 to keep searching:\x1b[32m ";
        cin >> go;
        getchar();
    }
    return 0;
}

string removespecialchar(string s) {
    for (int i = 0; i < s.size(); i++) {

        // Finding the character whose ASCII value fall under this range
        if (s[i] < 'A' || s[i] > 'Z' &&
            s[i] < 'a' || s[i] > 'z')
        {
            // erase function to erase the character
            s.erase(i, 1);
            i--;
        }
    }
    return s;
}

void song_attributes(AVL_node* node) { //displays details of a song
    cout << "\n\x1B[40m\x1B[35mName: \x1B[37m" << node->data.name << "\n\x1B[35mID: \x1B[37m" << node->data.id
        << "\n\x1B[35mAlbum: \x1B[37m" << node->data.album << "\n\x1B[35mAlbum ID: \x1B[37m" << node->data.album_id
        << "\n\x1B[35mArtist: \x1B[37m" << node->data.artists << "\n\x1B[35mArtist ID: \x1B[37m" << node->data.artist_ids
        << "\n\x1B[35mTrack number: \x1B[37m" << node->data.track_number << "\n\x1B[35mDisk number: \x1B[37m" << node->data.disc_number
        << "\n\x1B[35mTrack number: \x1B[37m" << node->data.track_number << "\n\x1B[35mDisk number: \x1B[37m" << node->data.disc_number
        << "\n\x1B[35mExplicit: \x1B[37m" << node->data.explicit1 << "\n\x1B[35mDanceability: \x1B[37m" << node->data.danceability
        << "\n\x1B[35mEnergy: \x1B[37m" << node->data.energy << "\n\x1B[35mKey: \x1B[37m" << node->data.key
        << "\n\x1B[35mLoudness: \x1B[37m" << node->data.loudness << "\n\x1B[35mMode: \x1B[37m" << node->data.mode
        << "\n\x1B[35mSpeechiness: \x1B[37m" << node->data.speechines << "\n\x1B[35mAcousticness: \x1B[37m" << node->data.acousticness
        << "\n\x1B[35mInstrument: \x1B[37m" << node->data.instrument << "\n\x1B[35mLiveness: \x1B[37m" << node->data.liveness
        << "\n\x1B[35mValence: \x1B[37m" << node->data.valence << "\n\x1B[35mTempo: \x1B[37m" << node->data.tempo
        << "\n\x1B[35mDuration(ms): \x1B[37m" << node->data.duration_ms << "\n\x1B[35mTime signature: \x1B[37m" << node->data.time_signature
        << "\n\x1B[35mYear: \x1B[37m" << node->data.year << "\n\x1B[35mRelease_date: \x1B[37m" << node->data.release_date << "\n";
}
