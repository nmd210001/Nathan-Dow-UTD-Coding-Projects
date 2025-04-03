package edu.utdallas.cs2336;

public interface CS2336Stack<T> {
    void push(Node<T> element);
    Node<T> pop();
    int getSize();
}
