package edu.utdallas.cs2336;

import edu.utdallas.cs2336.*;
import org.junit.jupiter.api.Test;

public class TestBinaryTree {
    private CS2336BinaryTree setUpTree() {
        // Tree looks like this
        //          50
        //       /      \
        //     40       53
        //    /       /    \
        //   30      51    70
        //     \         /     \
        //     35       55     71

        CS2336BinaryTree tree = new BinaryTreeImpl();
        tree.add(new Node<>(50));
        tree.add(new Node<>(40));
        tree.add(new Node<>(30));
        tree.add(new Node<>(35));
        tree.add(new Node<>(53));
        tree.add(new Node<>(51));
        tree.add(new Node<>(70));
        tree.add(new Node<>(55));
        tree.add(new Node<>(71));

        return tree;
    }

    @Test
    public void testSimpleAddition() {
        CS2336BinaryTree tree = new BinaryTreeImpl();
        tree.add(new Node<>(50));
        assert ( tree.getRoot() != null && tree.getRoot().getValue() == 50);
        assert ( tree.getRoot().getChildStatus() == Node.ChildStatus.NONE);
    }


    @Test
    public void testComplexAddition() {
        CS2336BinaryTree tree = setUpTree();
        Node<Integer> root = tree.getRoot();
        assert ( root.getValue() == 50);

        assert ( root.getLeftNode().getValue() == 40);
        assert ( root.getLeftNode().getRightNode() == null);
        assert ( root.getLeftNode().getLeftNode().getValue() == 30);
        assert ( root.getLeftNode().getLeftNode().getLeftNode() == null);
        assert ( root.getLeftNode().getLeftNode().getRightNode().getValue().equals(35));
        assert ( root.getLeftNode().getLeftNode().getRightNode().getChildStatus() == Node.ChildStatus.NONE);

        assert ( root.getRightNode().getValue() == 53);
        assert ( root.getRightNode().getLeftNode().getValue() == 51);
        assert ( root.getRightNode().getLeftNode().getChildStatus() == Node.ChildStatus.NONE);
        assert ( root.getRightNode().getRightNode().getValue() == 70);
        assert ( root.getRightNode().getRightNode().getLeftNode().getValue() == 55);
        assert ( root.getRightNode().getRightNode().getLeftNode().getChildStatus() == Node.ChildStatus.NONE);
        assert ( root.getRightNode().getRightNode().getRightNode().getValue() == 71);
        assert ( root.getRightNode().getRightNode().getRightNode().getChildStatus() == Node.ChildStatus.NONE);
    }

    @Test
    public void testFind() {
        CS2336BinaryTree tree = setUpTree();
        assert ( tree.find(10) == null);
        assert ( tree.find(33) == null);
        assert ( tree.find(54) == null);
        assert ( tree.find(72) == null);

        assert ( tree.find(50).getValue() == 50);
        assert ( tree.find(40).getValue() == 40);
        assert ( tree.find(30).getValue() == 30);
        assert ( tree.find(35).getValue() == 35);

        assert ( tree.find(53).getValue() == 53);
        assert ( tree.find(51).getValue() == 51);
        assert ( tree.find(70).getValue() == 70);
        assert ( tree.find(55).getValue() == 55);
        assert ( tree.find(71).getValue() == 71);
    }

    @Test
    public void testRemoveNonExistant() {
        CS2336BinaryTree tree = setUpTree();

        assert ( tree.remove(10) == null);
        assert ( tree.remove(48) == null);
        assert ( tree.remove(52) == null);
        assert ( tree.remove(74) == null);
    }

    @Test
    public void testRemoveLeaves() {
        CS2336BinaryTree tree = setUpTree();

        // Remove leaves
        Node<Integer> thirtyFive = tree.remove(35);
        System.out.println(thirtyFive.getValue());
        assert ( thirtyFive.getValue() == 35);
        assert ( thirtyFive.getChildStatus() == Node.ChildStatus.NONE);
        assert ( tree.getRoot().getLeftNode().getLeftNode().getChildStatus() == Node.ChildStatus.NONE);

        Node<Integer> fiftyFive = tree.remove(55);
        assert ( fiftyFive.getValue() == 55);
        assert ( (fiftyFive).getChildStatus() == Node.ChildStatus.NONE);
        assert ( tree.getRoot().getRightNode().getRightNode().getChildStatus() == Node.ChildStatus.RIGHT_ONLY);
        assert ( tree.getRoot().getRightNode().getRightNode().getRightNode().getValue() == 71);

        Node<Integer> seventyOne = tree.remove(71);
        assert ( seventyOne.getValue() == 71);
        assert ( seventyOne.getChildStatus() == Node.ChildStatus.NONE);
        assert ( tree.getRoot().getRightNode().getRightNode().getChildStatus() == Node.ChildStatus.NONE);
    }

    @Test
    public void testRemoveRoot() {
        CS2336BinaryTree tree = setUpTree();
        Node<Integer> root = tree.remove(50);
        assert ( root.getValue() == 50);
        assert ( root.getChildStatus() == Node.ChildStatus.NONE);
        assert ( tree.getRoot().getValue().equals(51));
        assert ( tree.getRoot().getLeftNode().getValue().equals(40));
        assert ( tree.getRoot().getRightNode().getValue().equals(53));
    }

    @Test
    public void testRemoveOthers() {
        CS2336BinaryTree tree = setUpTree();

        Node<Integer> removed = tree.remove(30);
        assert ( removed.getChildStatus() == Node.ChildStatus.NONE);
        assert ( tree.getRoot().getLeftNode().getLeftNode().getValue() == 35);

        removed = tree.remove(53);
        assert ( removed.getChildStatus() == Node.ChildStatus.NONE);
        assert ( removed.getValue() == 53);
        assert ( tree.getRoot().getRightNode().getValue() == 55);
        assert ( tree.getRoot().getRightNode().getLeftNode().getValue() == 51);
        assert ( tree.getRoot().getRightNode().getRightNode().getValue() == 70);
    }
}
