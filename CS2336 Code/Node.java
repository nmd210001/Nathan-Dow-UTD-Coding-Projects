package edu.utdallas.cs2336;

public class Node<T> {

    /**
     * enum for which children this node has; only needed for the binary tree
     */
    public enum ChildStatus {
        NONE,
        LEFT_ONLY,
        RIGHT_ONLY,
        BOTH
    }


    private Node<T> leftNode = null;
    private Node<T> rightNode = null;

    private Node<T> parentNode = null;

    private final T value;

    /**
     * Constructor that takes the value of this node
     * @param value Value to
     */
    public Node(T value) {
        this.value = value;
    }

    /**
     * "Copy" constructor that uses the value of another node
     * but DOES NOT copy the previous and next Nodes (as this new node shouldn't
     * be associated with the other node
     * @param other Node whose value (but not links) we want
     */
    public Node(Node<T> other) {
        this.value = other.value;
    }



    /**
     * Get the previous node
     * @return The previous node, or null if there isn't one
     */
    public Node<T> getLeftNode() {
        return leftNode;
    }

    /**
     * Set the previous node
     * @param leftNode New previous node; okay to be null
     */
    public void setLeftNode(Node<T> leftNode) {
        this.leftNode = leftNode;
    }

    /**
     * Get the next node
     * @return The next node, or null if there isn't one
     */
    public Node<T> getRightNode() {
        return rightNode;
    }

    /**
     * Set the next node
     * @param rightNode New next node; okay to be null
     */
    public void setRightNode(Node<T> rightNode) {
        this.rightNode = rightNode;
    }

    /**
     * Get the node's parent (if any). Note that this is only needed for the binary tree part
     * @return The node's parent (if any)
     */
    public Node<T> getParentNode() {
        return parentNode;
    }

    /**
     * Set the node's parent. Note that this is only needed for the binary tree part
     * @param parentNode The node's parent; fine if it is null
     */
    public void setParentNode(Node<T> parentNode) {
        this.parentNode = parentNode;
    }

    /**
     * Tells you what children (if any) this node has.
     * Note that you only need this in the binary tree part
     *
     * @return The current status (NONE (this is a leaf), LEFT_ONLY, RIGHT_ONLY, or BOTH)
     */
    public ChildStatus getChildStatus() {
        if ( leftNode == null) {
            if ( rightNode == null) {
                return ChildStatus.NONE;
            } else {
                return ChildStatus.RIGHT_ONLY;
            }
        } else {
            if ( rightNode == null) {
                return ChildStatus.LEFT_ONLY;
            }
            return ChildStatus.BOTH;
        }
    }

    /**
     * Get the value contained in this node
     * @return The value of the node
     */
    public T getValue() {
        return value;
    }


    /**
     * We consider "equality" here to be based on value, NOT on the previous/next nodes
     * @param other The other Node to check against
     * @return Do they have equal values?
     */
    @Override
    public boolean equals(Object other) {
        if ( other instanceof Node) {
            final Node<T> other1 = (Node<T>) other;
            return (value.equals(other1.value));
        }
        return false;
    }
}
