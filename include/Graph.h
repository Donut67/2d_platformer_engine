#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <iostream>

// Graph Definition
template <class vertex> 
class Graph {
public:  
	Graph() {}
	virtual ~Graph() {}

	virtual void AddVertex(const vertex& v) = 0;  
	// virtual void AddEdge(const vertex& v1, const vertex& v2) = 0; 
	virtual void RemoveVertex(const vertex& v) = 0;  
	// virtual void RemoveEdge(const vertex& v1, const vertex& v2) = 0; 

	virtual bool ExistsVertex(const vertex& v) const = 0; 
	// virtual bool ExistsEdge(const vertex& v1, const vertex& v2) const = 0;
	virtual bool isEmpty() const = 0; 
};

template <class vertex, class edge> 
class LabeledGraph : public Graph<vertex> {
private:
	struct label;
	struct node {
		vertex content;
		node* next;
		label* edg;
	};
	struct label {
		edge content;
		node* desti;
		label* next;
	};

	node* findVertex(const vertex& v) const {
		node* result = begin;
		bool found = false;

		while (!found && result != NULL) {
			found = result->content == v;
			if (!found) result = result->next;
		}

		return result;

	}
	void copy(const LabeledGraph& o) {
		begin = o.begin;
	}
	void free() {

	}

	node* begin;
public:  
	LabeledGraph() {
		begin = NULL;
	}
	LabeledGraph(const LabeledGraph& o) {
		begin = NULL;
		copy(o);
	}
	~LabeledGraph() {
		free();
	}

	void AddVertex(const vertex& v) {
		if(!ExistsVertex(v)){
			node* newnode = new node;
			newnode->content = v;
			newnode->next = NULL;
			newnode->edg = NULL;

			if (begin == NULL) begin = newnode;
			else {
				node* aux = begin;
				while (aux->next != NULL) aux = aux->next;
				aux->next = newnode;
			}
		}
	}
	void AddEgde(const vertex& v1, const vertex& v2, const edge& e) {
		if(!ExistsEgde(v1, v2)){
			node* n1 = findVertex(v1);
			node* n2 = findVertex(v2);
			if (n1 != NULL && n2 != NULL) {
				label* newlabel = new label;
				newlabel->content = e;
				newlabel->next = NULL;
				newlabel->desti = n2;

				if (n1->edg == NULL) n1->edg = newlabel;
				else {
					label* aux = n1->edg;

					while (aux->next != NULL) aux = aux->next;

					aux->next = newlabel;
				}
			}
		}
	}
	void ChangeVertex(const vertex& v1, const vertex& v2) {
		node* n = findVertex(v1);
		if (n != NULL) n->content = v2;
	}
	void ChangeEdge(const vertex& v1, const vertex& v2, const edge& e) {
		node* n1 = findVertex(v1);
		node* n2 = findVertex(v2);
		if (n1 != NULL && n2 != NULL) {
			label* actual = n1->edg;
			bool found = false;

			while (!found && actual != NULL) {
				found = actual->desti == n2;
				if (!found) actual = actual->next;
			}

			if (found) actual->content = e;
		}
	}
	void ChangeDestination(const vertex& v1, const vertex& v2, const edge& e) {
		node* n1 = findVertex(v1);
		node* n2 = findVertex(v2);
		if (n1 != NULL && n2 != NULL) {
			label* actual = n1->edg;
			bool found = false;

			while (!found && actual != NULL) {
				found = actual->content == e;
				if (!found) actual = actual->next;
			}

			if (found) actual->desti = n2;
		}
	}

	void RemoveVertex(const vertex& v) {
		if(ExistsVertex(v)) {
			node* actual = begin;
			node* prev = actual;
			bool found = false;

			while (!found && actual != NULL) {
				found = actual->content == v;
				if (!found) {
					prev = actual;
					actual = actual->next;
				}
			}
			node* aux = begin;
			while (aux != NULL) {
				if(aux->content != actual->content) RemoveEgde(aux->content, v);
				aux = aux->next;
			}
			if(actual == prev || found) {
				if(actual == prev) begin = actual->next;
				else prev->next = actual->next;
				delete actual;
			}
		}
	}
	void RemoveEgde(const vertex& v1, const vertex& v2) {
		if(ExistsEgde(v1, v2)){
			node* n1 = findVertex(v1);
			node* n2 = findVertex(v2);
			label* actual = n1->edg;
			label* prev = actual;
			bool found = false;

			while (!found && actual != NULL) {
				found = actual->desti == n2;
				if (!found) {
					prev = actual;
					actual = actual->next;
				}
			}

			if(actual == prev || found) {
				if(actual == prev) n1->edg = actual->next;
				else if (found) prev->next = actual->next;
				delete actual;
			}
		}
	}

	bool ExistsVertex(const vertex& v) const {
		return findVertex(v) != NULL;
	}
	bool ExistsEgde(const vertex& v1, const vertex& v2) const {
		node* n1 = findVertex(v1);
		node* n2 = findVertex(v2);
		if (n1 != NULL && n2 != NULL) {
			label* result = n1->edg;
			bool found = false;

			while (!found && result != NULL) {
				found = result->desti == n2;
				if (!found) result = result->next;
			}

			return found;
		}
		else return false;
	}

	std::vector<vertex> GetVertex() const {
		std::vector<vertex> result;
		node* aux = begin;

		while (aux != NULL) {
			result.push_back(aux->content);
			aux = aux->next;
		}

		return result;
	}
	std::vector<edge> GetEdges(const vertex& v1) const {
		std::vector<edge> result;

		node* n1 = findVertex(v1);
		if (n1 != NULL) {
			label* aux = n1->edg;

			while (aux != NULL) {
				result.push_back(aux->content);
				aux = aux->next;
			}
		}

		return result;
	}
	vertex GetNextVertex(const vertex& v1, const edge& e) const {
		vertex result;
		node* n1 = findVertex(v1);
		if (n1 != NULL) {
			label* aux = n1->edg;
			bool found = false;

			while (!found && aux != NULL) {
				found = aux->content == e;
				if(!found) aux = aux->next;
			}

			if (found) result = aux->desti->content;
		}
		return result;
	}

	bool isEmpty() const {
		return begin == NULL;
	}
};
#endif // !GRAPH_H