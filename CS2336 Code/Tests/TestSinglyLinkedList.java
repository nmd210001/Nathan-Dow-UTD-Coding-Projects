package edu.utdallas.cs2336;

import edu.utdallas.cs2336.CS2336List;
import edu.utdallas.cs2336.Node;
import edu.utdallas.cs2336.SinglyLinkedList;
import org.junit.jupiter.api.Test;

public class TestSinglyLinkedList {

    private CS2336List<Integer> add123() {
        CS2336List<Integer> list = new SinglyLinkedList<>();
        list.addToFront(new Node<>(1));
        list.addToBack(new Node<>(3));
        list.addAt(new Node<>(2), 1);

        return list;
    }

    @Test
    public void testAddOne() {
        Node<Integer> integerNode = new Node<>(5);
        SinglyLinkedList<Integer> list = new SinglyLinkedList<>();
        assert (list.getSize() == 0);

        list.addToFront(integerNode);
        assert (list.getSize() == 1);
        assert (list.getFirst().equals(integerNode));
        assert (list.getLast().equals(integerNode));
        assert (list.getFirst().getRightNode() == null);
    }

    @Test
    public void testAddSeveral() {
        CS2336List<Integer> list = add123();
        assert (list.getSize() == 3);
        assert (list.getFirst().getValue() == 1);
        assert (list.getLast().getValue() == 3);

        assert (list.getAt(0).getValue() == 1);
        assert (list.getAt(1).getValue() == 2);
        assert (list.getAt(2).getValue() == 3);

        assert (list.getAt(0).getRightNode().getValue() == 2);
        assert (list.getAt(1).getRightNode().getValue() == 3);
        assert (list.getLast().getRightNode() == null);

        try {
            list.addAt(new Node<>(5), 4);
            throw new RuntimeException("Test failed!");
        } catch ( IndexOutOfBoundsException e) {
            // This should happen :)
        }
    }

    @Test
    public void testRemoval() {
        CS2336List<Integer> list = new SinglyLinkedList<>();
        assert (list.removeFront() == null);
        assert (list.removeBack() == null);

        list = add123();
        Node<Integer> removedNode = list.removeFront();
        assert (removedNode.getValue() == 1);
        assert (removedNode.getRightNode() == null);
        assert (list.getFirst().getValue() == 2);
        assert (list.getFirst().getRightNode().getValue() == 3);
        assert (list.getSize() == 2);

        list.addToFront(new Node<>(4));// this is my problem line
        removedNode = list.removeBack();
        assert (removedNode.getValue() == 3);
        assert (list.getLast().getValue() == 2);
        assert (list.getLast().getRightNode() == null);
        assert (list.getSize() == 2);

        try {
            list.removeAt(2);
            throw new RuntimeException("Test failed!");
        } catch ( IndexOutOfBoundsException e) {
            // Should happen :)
        }
        list.addToFront(new Node<>(5));
        removedNode = list.removeAt(1);
        assert (removedNode.getValue() == 4);
        assert (removedNode.getRightNode() == null);
        assert (list.getSize() == 2);
        assert (list.getFirst().getValue() == 5);
        assert (list.getFirst().getRightNode().getValue() == 2);
        assert (list.getLast().getValue() == 2);

        removedNode = list.removeBack();
        assert (removedNode.getValue() == 2);
        assert (list.getSize() == 1);
        assert (list.getFirst().equals(list.getLast()));
    }
}
