package edu.utdallas.cs2336;

/**
 * Interface for a singly-linked list
 * @param <T>
 */
public interface CS2336List<T> {
    void addToFront(Node<T> newFirst);
    void addToBack(Node<T> newLast);

    /**
     * Add an item at the specified index
     * @param newItem Item to add
     * @param index Index to add it
     */
    void addAt(Node<T> newItem, final int index);

    /**
     * Get the first node
     * @return The first node, or null if the list is empty
     */
    Node<T> getFirst();

    /**
     * Get the last node
     * @return The last node, or null if the list is empty
     */
    Node<T> getLast();

    /**
     * Get the size of the list
     * @return Size of the list
     */
    int getSize();

    /**
     * Get the node at this index
     * @param index Index to check
     * @return Node at that index
     */
    Node<T> getAt(int index);

    /**
     * Remove the first node
     * @return The removed node (if any)
     */
    Node<T> removeFront();

    /**
     * Remove the last node
     * @return The removed node (if any)
     */
    Node<T> removeBack();

    /**
     * Remove the node at this index
     * @param index The index to remove
     * @return The removed node
     */
    Node<T> removeAt(int index);

}
