#include "core.h"
#include <vector>
using namespace std;

class Vertex {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        vector<std::pair<int,float>> attachments;
        //where to put binding matrices

};