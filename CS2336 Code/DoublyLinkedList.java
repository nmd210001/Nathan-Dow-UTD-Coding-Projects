package edu.utdallas.cs2336;

public class DoublyLinkedList <T> extends SinglyLinkedList<T> {

    @Override
    public void addToFront(Node<T> newFirst) {
        //System.out.println("Going into addToFront in the double linkList");
        super.addToFront(newFirst);
        if (newFirst.getRightNode() != null) {
            newFirst.getRightNode().setLeftNode(newFirst);
        }
        //System.out.println("Exiting addToFront in the double linkList\n");
    }

    @Override
    public void addToBack(Node<T> newLast) {
        //System.out.println("Going into addToBack in the double linkList");
        if (getFirst() == null) {
            super.addToBack(newLast);
        } else {
            Node<T> lastNode = getLast();
            lastNode.setRightNode(newLast);
            newLast.setLeftNode(lastNode);
            newLast.setRightNode(null);
            size++;
        }
        //System.out.println("Exiting addToBack in the double linkList\n");
    }

    @Override
    public void addAt(Node<T> newItem, int index) {
        //System.out.println("Going into addAT in the double linkList");
        if (index < 0 || index > size)
            throw new IndexOutOfBoundsException("Out of Bounds Exception");
        if (index == 0) {
            addToFront(newItem);
        } else if (index == getSize()) {
            addToBack(newItem);
        } else {
            Node<T> currentNode = getAt(index);
            super.addAt(newItem, index);
            currentNode.getLeftNode().setRightNode(newItem);
            newItem.setLeftNode(currentNode.getLeftNode());
            newItem.setRightNode(currentNode);
            currentNode.setLeftNode(newItem);
        }
        //System.out.println("Exiting addAT in the double linkList\n");
    }

    @Override
    public Node<T> removeFront() {
        //System.out.println("Going into removeFront in the double linkList");
        Node<T> removedNode = super.removeFront();
        if (getFirst() != null) {
            getFirst().setLeftNode(null);
        }
        //System.out.println("Exiting addToBack in the double linkList\n");
        return removedNode;
    }

    @Override
    public Node<T> removeBack() {
        //System.out.println("Going into removeBack in the double linkList");
        if (getSize() <= 1) {
            //System.out.println("Exiting removeBack in the double linkList\n");
            return super.removeBack();
        } else {
            Node<T> removeNode = super.removeBack();
            removeNode.setLeftNode(null);
            return removeNode;
        }
    }

    @Override
    public Node<T> removeAt(int index) {
        System.out.println("Going into removeAt in the double linkList");
        if (index < 0 || index >= size)
            throw new IndexOutOfBoundsException("Out of Bounds Exception");
        if (index == 0) {
            System.out.println("Exiting removeAt in the double linkList\n");
            return removeFront();
        } else if (index == getSize() - 1) {
            System.out.println("Exiting removeAt in the double linkList\n");
            return removeBack();
        } else {
            Node<T> removedNode = getAt(index);
            if (removedNode.getRightNode() != null) {
                removedNode.getRightNode().setLeftNode(removedNode.getLeftNode());
            }
            super.removeAt(index);
            removedNode.setLeftNode(null);
            System.out.println("Exiting removeAt in the double linkList\n");
            return removedNode;
        }
    }
}
