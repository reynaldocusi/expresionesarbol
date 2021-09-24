#include <string>
#include <iostream>
#include <cassert>
#include <math.h>
#include <stack>
#include <map>
// # BinaryTree > ExpressionTree > ("5*3 + 2 - 6/3")
//                         +
//                                 -
//                 *          2         /
//             5       3           6       3

using namespace std;

struct ExprNode
{
  ExprNode *left{nullptr};
  ExprNode *right{nullptr};
  virtual double compute() = 0;
  virtual string get_val() = 0;
};

struct OpNode : ExprNode
{
  OpNode(char op, ExprNode *left, ExprNode *right) : ExprNode(), operator_(op)
  {
    this->left = left;
    this->right = right;
  }

  double compute() override
  {
    auto lhs = this->left->compute();
    auto rhs = this->right->compute();
    switch (operator_)
    {
    case '+':
      return lhs + rhs;
    case '-':
      return lhs - rhs;
    case '*':
      return lhs * rhs;
    case '/':
      return lhs / rhs;
    case '^':
      return pow(lhs, rhs);
    default:
      assert(false);
      break;
    }
  }

  string get_val() override
  {
    return string(1, operator_);
  }
  char operator_;
};

//// DFS -> stack -> LIFO
//post-order = [travel(travel.left) + travel(travel.right) + root]
//pre-order = [root + travel(travel.left) + travel(travel.right)]
//in-order = [travel(travel.left) + root + travel(travel.right)]

// BFS -> queue -> FIFO
// level transversal
// => +,
// * -
// 5, 3, 2, /
// 6 3

struct NumberNode : ExprNode
{
  NumberNode(double value) : ExprNode(), value_(value)
  {
  }

  double compute() override
  {
    return value_;
  }

  string get_val() override
  {
    return to_string(value_);
  }
  double value_;
};

struct ExpressionTree
{
  ExpressionTree(ExprNode *root) : root_(root)
  {
  }

  double compute()
  {
    return root_->compute();
  }
  ExprNode *root_;
};

ExpressionTree BuildExpressionTree(string expr){
  //Usaremos 2 stack uno para operadores y otro para los nodos
  stack<char> s_operadores;
  stack<ExprNode *> s_nodos;

  //asignando las prioridades a los operadores
  map<char, int> prio;
  prio.insert({'+', 1});
  prio.insert({'-', 1});
  // los operadores * y / tienen prioridad 2 
  prio.insert({'*', 2});
  prio.insert({'/', 2});
  //prioridad 3, se resulve en ese orden
  prio.insert({'^', 3});

  // Recorremos la expresion 
  for (int i = 0; i < expr.length(); i++){
    // si encontramos un digito 
    if (isdigit(expr[i])){
      //seguimos recorriendo
      std::string num = "";
      while (isdigit(expr[i])) {
        num.push_back(expr[i++]);
      }
      i--;
      //creamos un nodo y agregamos al stack
      NumberNode *t = new NumberNode(stoi(num));
      s_nodos.push(t);
    } else {
      // agregamos a la pila de operadores segun su prioridad 
      while (!s_operadores.empty() && prio[s_operadores.top()] >= prio[expr[i]]) {
        // sacamos el primer nodo 
        ExprNode *first = s_nodos.top();
        s_nodos.pop();
        // sacamos el segundo nodo 
        ExprNode *second = s_nodos.top();
        s_nodos.pop();

        // obtenemos el operador de nuestro stack de operadores
        char op = s_operadores.top();
        s_operadores.pop();

        // creamos un nodo con hijo izquierdo al segundo nodo e hijo derecho al primer nodo
        OpNode *new_node = new OpNode(op, second, first);
        // agregamos el nuevo nodo al stack de nodos 
        s_nodos.push(new_node);
      }
      // el operador que estabamos leyendo se agrega al stack de operadores 
      s_operadores.push(expr[i]);
    }
  }

  // Si al final de recorrer la cadena aun en nuestro stack de operadores contiene operadores, se procesa hasta que no haya operadores
  while (!s_operadores.empty()) {
    ExprNode *first = s_nodos.top();
    s_nodos.pop();

    ExprNode *second = s_nodos.top();
    s_nodos.pop();

    char op = s_operadores.top();
    s_operadores.pop();

    OpNode *new_node = new OpNode(op, second, first);
    s_nodos.push(new_node);
  }

  //se devuvel el ultimo nodo que aun queda en nuestro stack de nodos
  return ExpressionTree(s_nodos.top());
}


int main()
{
  ExpressionTree tree = BuildExpressionTree("3*5^2+2");
  std::cout << tree.compute() << std::endl;
}