package ru.nsu.fit.g19202.dmakogon.task2.calc;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.core.config.Configurator;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.StackCalculatorException;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class StackCalculatorTests
{
    private static final double DOUBLE_DELTA = 1e-10;

    @BeforeEach
    void turnOffLogging()
    {
        Configurator.setRootLevel(Level.OFF);
    }

    @Test
    void executesSimpleProgramCorrectly() throws IOException, StackCalculatorException
    {
        String program = """
                DEFINE a 4
                PUSH a
                SQRT
                PRINT
                """;

        double expectedResult = 2;

        testSingleValueOutput(program, expectedResult);
    }

    @Test
    void executesEmptyProgram()
    {
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

        assertDoesNotThrow(() -> new StackCalculator(stringToStream(""), outputStream).execute());

        assertEquals("", outputStream.toString());
    }

    @Test
    void continuesExecutingOnWrongInput() throws IOException, StackCalculatorException
    {
        String wrongProgram = """
                PUSH 1.14
                SQRT
                DEF x
                SUB
                PUS
                PUSH 10
                +
                PRINT
                """;

        double expectedResult = Math.sqrt(1.14d) + 10;

        testSingleValueOutput(wrongProgram, expectedResult);
    }

    private InputStream stringToStream(String s)
    {
        return new ByteArrayInputStream(s.getBytes(StandardCharsets.UTF_8));
    }

    private void testSingleValueOutput(String inputProgram, double expected) throws IOException, StackCalculatorException
    {
        InputStream iStr = stringToStream(inputProgram);
        ByteArrayOutputStream oStr = new ByteArrayOutputStream();

        StackCalculator stackCalc = new StackCalculator(iStr, oStr);

        stackCalc.execute();

        // remove any line breaks
        String actualStr = oStr.toString().replaceAll("\\R", "");

        assertEquals(expected, Double.parseDouble(actualStr), DOUBLE_DELTA);
    }
}
