package edu.utdallas.cs2336;

import edu.utdallas.cs2336.*;
import org.junit.jupiter.api.Test;

public class TestQueue {
    private static final String FIRST_STRING = "ABC";
    private static final String SECOND_STRING = "DEF";
    private static final String THIRD_STRING = "GHI";

    @Test
    public void testQueue() {
        CS2336Queue<String> stack = new QueueImpl<>();
        stack.push(new Node<>(FIRST_STRING));
        stack.push(new Node<>(SECOND_STRING));
        stack.push(new Node<>(THIRD_STRING));

        assert (stack.pop().getValue().equals(FIRST_STRING));
        assert (stack.pop().getValue().equals(SECOND_STRING));
        assert (stack.pop().getValue().equals(THIRD_STRING));
        assert (stack.getSize() == 0);
    }
}
