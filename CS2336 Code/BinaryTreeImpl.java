package edu.utdallas.cs2336;

public class BinaryTreeImpl implements CS2336BinaryTree {
   private Node<Integer> root;
    @Override
    public Node<Integer> getRoot() {
        return root;
    }

    @Override
    public void add(Node<Integer> node) {
        root = addRec(root, node);
    }

    private Node<Integer> addRec(Node<Integer> current, Node<Integer> newNode) {
        if (current == null) {
            newNode.setParentNode(null); // Set parent to null when adding a new node
            return newNode;
        }

        if (newNode.getValue() < current.getValue()) {
            current.setLeftNode(addRec(current.getLeftNode(), newNode));
            if (current.getLeftNode() != null) {
                current.getLeftNode().setParentNode(current); // Set parent when adding to the left
            }
        } else if (newNode.getValue() > current.getValue()) {
            current.setRightNode(addRec(current.getRightNode(), newNode));
            if (current.getRightNode() != null) {
                current.getRightNode().setParentNode(current); // Set parent when adding to the right
            }
        }
        return current;
    }

    @Override
    public Node<Integer> find(int value) {
        return findRec(root, value);
    }

    private Node<Integer> findRec(Node<Integer> current, int value) {
        if (current == null || current.getValue() == value)
            return current;
        if (value < current.getValue())
            return findRec(current.getLeftNode(), value);
        else
            return findRec(current.getRightNode(), value);
    }

    @Override
    public Node<Integer> remove(int value) {
        Node<Integer> found = find(value);
        if (found == null)
            return null;

        Node.ChildStatus childStatus = found.getChildStatus();

        if (childStatus == Node.ChildStatus.NONE) {
            // No children
            // Set parent's reference to this node to null
            if (found.getParentNode() != null) {
                if (found.getParentNode().getLeftNode() == found)
                    found.getParentNode().setLeftNode(null);
                else
                    found.getParentNode().setRightNode(null);
            } else root = null;
        } else if (childStatus == Node.ChildStatus.RIGHT_ONLY) {
            // Right child only
            Node<Integer> rep = found.getRightNode();
            rep.setParentNode(found.getParentNode());
            if (found.getParentNode() != null) {
                if (found.getParentNode().getLeftNode() == found)
                    found.getParentNode().setLeftNode(rep);
                else
                    found.getParentNode().setRightNode(rep);
            } else root = rep; //makes sure to set the root to rep if the replace value is the root

            // clear all the references in found
            found.setParentNode(null);
            found.setLeftNode(null);
            found.setRightNode(null);

            return found;
        } else if (childStatus == Node.ChildStatus.LEFT_ONLY) {
            // Left child only
            Node<Integer> rep = found.getLeftNode();
            found.getLeftNode().setParentNode(found.getParentNode());
            if (found.getParentNode() != null) {
                if (found.getParentNode().getLeftNode() == found)
                    found.getParentNode().setLeftNode(rep);
                else
                    found.getParentNode().setRightNode(rep);
            } else root = rep; //makes sure to set the root to rep if the replace value is the root

            // clear all the references in found
            found.setParentNode(null);
            found.setLeftNode(null);
            found.setRightNode(null);

            return found;
        } else {
            //Case if node has two children
            Node<Integer> min = findMin(found.getRightNode()); //saved min value that's greater than found
            remove(min.getValue());// deletes the min value by turning it to null
            found.getRightNode().setParentNode(min); //replaces the node being replaced with saved min node
            if (found.getParentNode() != null) {
                if (found.getParentNode().getLeftNode() == found)
                    found.getParentNode().setLeftNode(min);
                else
                    found.getParentNode().setRightNode(min);
            } else root = min;

            // Update children references for 'min'
            min.setLeftNode(found.getLeftNode());
            if (found.getLeftNode() != null) {
                found.getLeftNode().setParentNode(min);
            }
            min.setRightNode(found.getRightNode());
            if (found.getRightNode() != null) {
                found.getRightNode().setParentNode(min);
            }
            // clear all the references in found
            found.setParentNode(null);
            found.setLeftNode(null);
            found.setRightNode(null);
        }
        return found;
    }

    private Node<Integer> findMin(Node<Integer> node) {
        while (node.getLeftNode() != null) {
            node = node.getLeftNode();
        }
        return node;
    }
}
