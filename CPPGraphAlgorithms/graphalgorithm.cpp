/*******************************************************************************
 * Name          : graphalgorithm.cpp
 * Author        : Brian S. Borowski
 * Version       : 1.1
 * Date          : September 27, 2014
 * Last modified : October 16, 2017
 * Description   : Class that implements standard graph traversal algorithms.
 ******************************************************************************/
#include "graphalgorithm.h"
#include "graph.h"
#include "weightedgraph.h"
#include "vertex.h"
#include "minheap.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <stdexcept>

using namespace std;

static void breadth_first_search_visit(
        vertex_id vertex, const Graph * const graph, bool *visited,
        vector<vertex_id> &order) {

    queue<vertex_id> q;
    q.push(vertex);
    while (!q.empty()) {
        vertex_id front = q.front();
        order.push_back(front);
        vector<vertex_id> adjacent = graph->get_adjacent(front);
        for (const auto &vertex : adjacent) {
            if (!visited[vertex]) {
                visited[vertex] = true;
                q.push(vertex);
            }
        }
        q.pop();
    }
}

vector<vertex_id> breadth_first_search(const Graph * const graph) {
    vector<vertex_id> order, vertices = graph->get_vertices();
    size_t num_vertices = static_cast<size_t>(graph->get_highest_vertex_id());
    order.reserve(num_vertices);
    bool *visited = new bool[num_vertices + 1];
    fill(visited, visited + num_vertices + 1, false);
    for (const auto &vertex : vertices) {
        if (!visited[vertex]) {
            visited[vertex] = true;
            breadth_first_search_visit(vertex, graph, visited, order);
        }
    }
    delete [] visited;
    return order;
}

static void depth_first_search_visit(
        vertex_id vertex, const Graph * const graph, bool *visited,
        vector<vertex_id> &order) {

    visited[vertex] = true;
    order.push_back(vertex);
    vector<vertex_id> adjacent = graph->get_adjacent(vertex);
    for (const auto &vertex : adjacent) {
        if (!visited[vertex]) {
            depth_first_search_visit(vertex, graph, visited, order);
        }
    }
}

vector<vertex_id> depth_first_search(const Graph * const graph) {
    vector<vertex_id> order, vertices = graph->get_vertices();
    size_t num_vertices = static_cast<size_t>(graph->get_highest_vertex_id());
    order.reserve(num_vertices);
    bool *visited = new bool[num_vertices + 1];
    fill(visited, visited + num_vertices + 1, false);
    for (const auto &vertex : vertices) {
        if (!visited[vertex]) {
            depth_first_search_visit(vertex, graph, visited, order);
        }
    }
    delete [] visited;
    return order;
}

vector<vertex_id> top_sort(const Graph * const graph) {
    vector<vertex_id> order, vertices = graph->get_vertices();
    size_t num_vertices = static_cast<size_t>(graph->get_highest_vertex_id());
    order.reserve(num_vertices);
    int *in_degrees = new int[num_vertices + 1];
    fill(in_degrees, in_degrees + num_vertices + 1, -1);

    for (const auto &vertex : vertices) {
        in_degrees[vertex] = 0;
    }

    priority_queue< vertex_id, vector<vertex_id>,
        greater<vertex_id> > zero_indegree_nodes;

    for (size_t i = 1; i <= num_vertices; ++i) {
        vector<vertex_id> adjacent = graph->get_adjacent(i);
        for (const auto &vertex : adjacent) {
            ++in_degrees[vertex];
        }
    }

    for (size_t i = 1; i <= num_vertices; ++i) {
        if (in_degrees[i] == 0) {
            zero_indegree_nodes.push(i);
        }
    }

    while (!zero_indegree_nodes.empty()) {
        vertex_id n = zero_indegree_nodes.top();
        zero_indegree_nodes.pop();
        order.push_back(n);
        vector<vertex_id> adjacent = graph->get_adjacent(n);
        for (const auto &vertex : adjacent) {
            --in_degrees[vertex];
            if (in_degrees[vertex] == 0) {
                zero_indegree_nodes.push(vertex);
            }
        }
    }
    for (size_t i = 1; i <= num_vertices; ++i) {
        if (in_degrees[i] > 0) {
            order.clear();
            break;
        }
    }

    delete [] in_degrees;
    return order;
}

static string get_path(
        const Vertex &src, const Vertex &dst, Vertex vertices[]) {
    vector<vertex_id> path;
    Vertex *v = const_cast<Vertex*>(&dst);
    if (v->distance == UINT_MAX) {
        ostringstream oss;
        oss << "No path exists from vertex " << src.id << " to vertex "
            << dst.id << ".";
        return oss.str();
    }

    do {
        path.push_back(v->id);
        v = &vertices[v->predecessor_id];
    } while (v->id != 0);

    ostringstream oss;
    for (int i = path.size() - 1; i >= 0; --i) {
        oss << path[i];
        if (i != 0) {
            oss << " -> ";
        }
    }
    return oss.str();
}

static size_t get_vertex_index(vertex_id id,
        const MinHeap<Vertex*, distance_comparator> &queue) {
    for (size_t i = 0; i < queue.size(); ++i) {
        if (queue.at(i)->id == id) {
            return i;
        }
    }
    // This should not happen.
    throw runtime_error("Vertex ID not found.");
}

static void relax(Vertex *u, Vertex *v, const WeightedGraph * const graph,
        MinHeap<Vertex*, distance_comparator> &queue) {
    uint_t distance = u->distance + graph->get_weight(u->id, v->id);
    if (distance < v->distance) {
        v->distance = distance;
        v->predecessor_id = u->id;
        queue.decrease_key(get_vertex_index(v->id, queue), v, false);
    }
}

pair<uint_t, string> shortest_path(
        vertex_id src, vertex_id dst, const WeightedGraph * const graph) {
    MinHeap<Vertex*, distance_comparator> queue;
    Vertex *vertices =
            new Vertex[static_cast<size_t>(graph->get_highest_vertex_id()) + 1];
    const vector<vertex_id> vertex_ids = graph->get_vertices();

    for (const auto vertex_id : vertex_ids) {
        if (vertex_id != src) {
            // Make all predecessors 0 and distances +inf.
            vertices[vertex_id] = Vertex(vertex_id, 0, UINT_MAX);
        } else {
            // Start the src vertex with a distance of 0.
            vertices[vertex_id] = Vertex(vertex_id);
        }
        queue.insert(&vertices[vertex_id]);
    }

    while (!queue.empty()) {
        try {
            Vertex *u = queue.extract();
            vector<vertex_id> adjacent = graph->get_adjacent(u->id);
            for (const auto &vertex : adjacent) {
                relax(u, &vertices[vertex], graph, queue);
            }
        } catch (const heap_exception &he) { }
    }

    pair<uint_t, string> result(
            vertices[dst].distance,
            get_path(vertices[src], vertices[dst], vertices));
    delete [] vertices;
    return result;
}
