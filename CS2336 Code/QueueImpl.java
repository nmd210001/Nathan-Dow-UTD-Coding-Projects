package edu.utdallas.cs2336;

public class QueueImpl<T> extends SinglyLinkedList<T> implements CS2336Queue<T> {
    @Override
    public void push(Node<T> element) {
        this.addToBack(element);
    }

    @Override
    public Node<T> pop() {
      return this.removeFront();
    }
}
