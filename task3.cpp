#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Edge information: cost of travelling between two star systems
class TravelRoute {
public:
    float travel_cost;
    TravelRoute(float cost = 0) { travel_cost = cost; }
};

// A node of the M-ary tree
class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    float x, y, z;
    bool is_habitable;

    vector<StarSystem*> children;   // child star systems
    vector<TravelRoute> routes;     // routes[i] = route to children[i]

    StarSystem(int id, string n, int planets, float px, float py, float pz, bool habitable) {
        system_id = id;
        name = n;
        num_planets = planets;
        x = px;
        y = py;
        z = pz;
        is_habitable = habitable;
    }
};

class Galaxy {
private:
    int M;              // max children per node
    StarSystem* root;

    // number of nodes in the subtree rooted at x
    int subtree_size(StarSystem* x) const {
        if (x == NULL) return 0;
        int total = 1;
        for (int i = 0; i < (int)x->children.size(); i++)
            total += subtree_size(x->children[i]);
        return total;
    }

    void delete_tree(StarSystem* x) {
        if (x == NULL) return;
        for (int i = 0; i < (int)x->children.size(); i++)
            delete_tree(x->children[i]);
        delete x;
    }

public:
    // Overloaded constructors
    Galaxy(int m) {                         // empty galaxy
        M = m;
        root = NULL;
    }
    Galaxy(int m, StarSystem* r) {          // galaxy with a root
        M = m;
        root = r;
    }
    ~Galaxy() { delete_tree(root); }

    StarSystem* get_root() const { return root; }

    // Connect child to parent with a route. Returns false if parent is full.
    bool add_route(StarSystem* parent, StarSystem* child, float cost) {
        if ((int)parent->children.size() >= M) return false;
        parent->children.push_back(child);
        parent->routes.push_back(TravelRoute(cost));
        return true;
    }

    // b) Balanced: for every node, child subtree sizes differ by at most 1
    bool is_balanced(StarSystem* x) const {
        if (x == NULL || x->children.empty()) return true;

        int smallest = subtree_size(x->children[0]);
        int largest = smallest;
        for (int i = 1; i < (int)x->children.size(); i++) {
            int s = subtree_size(x->children[i]);
            if (s < smallest) smallest = s;
            if (s > largest) largest = s;
        }
        if (largest - smallest > 1) return false;

        // every child subtree must also be balanced
        for (int i = 0; i < (int)x->children.size(); i++)
            if (!is_balanced(x->children[i])) return false;
        return true;
    }

    // c) Count leaf nodes in the subtree rooted at x
    int count_leaves(StarSystem* x) const {
        if (x == NULL) return 0;
        if (x->children.empty()) return 1;
        int total = 0;
        for (int i = 0; i < (int)x->children.size(); i++)
            total += count_leaves(x->children[i]);
        return total;
    }
};

int main() {
    // Galaxy with M = 3 and root "Sol"
    StarSystem* sol = new StarSystem(1, "Sol", 8, 0, 0, 0, true);
    Galaxy galaxy(3, sol);

    StarSystem* alpha = new StarSystem(2, "Alpha Centauri", 3, 4, 1, 2, true);
    StarSystem* sirius = new StarSystem(3, "Sirius", 2, 8, 3, 1, false);
    StarSystem* vega = new StarSystem(4, "Vega", 5, 25, 7, 9, false);
    galaxy.add_route(sol, alpha, 4.3f);
    galaxy.add_route(sol, sirius, 8.6f);
    galaxy.add_route(sol, vega, 25.0f);

    StarSystem* proxima = new StarSystem(5, "Proxima", 1, 4, 2, 2, true);
    StarSystem* luyten = new StarSystem(6, "Luyten", 4, 9, 3, 1, false);
    StarSystem* altair = new StarSystem(7, "Altair", 2, 26, 8, 9, true);
    galaxy.add_route(alpha, proxima, 0.2f);
    galaxy.add_route(sirius, luyten, 1.5f);
    galaxy.add_route(vega, altair, 3.0f);

    cout << "Tree 1 (Sol -> 3 systems, each with 1 child)" << endl;
    cout << "  Leaf nodes : " << galaxy.count_leaves(galaxy.get_root()) << endl;
    cout << "  Balanced   : " << (galaxy.is_balanced(galaxy.get_root()) ? "Yes" : "No") << endl;

    // Make one branch much bigger: Proxima -> Tau -> Wolf
    StarSystem* tau = new StarSystem(8, "Tau Ceti", 4, 5, 5, 5, true);
    StarSystem* wolf = new StarSystem(9, "Wolf 359", 0, 6, 6, 6, false);
    galaxy.add_route(proxima, tau, 2.0f);
    galaxy.add_route(tau, wolf, 1.0f);

    cout << "\nTree 2 (Alpha Centauri branch now has 4 nodes, others 2)" << endl;
    cout << "  Leaf nodes : " << galaxy.count_leaves(galaxy.get_root()) << endl;
    cout << "  Balanced   : " << (galaxy.is_balanced(galaxy.get_root()) ? "Yes" : "No") << endl;

    cout << "\nLeaves under Sirius: " << galaxy.count_leaves(sirius) << endl;

    return 0;
}