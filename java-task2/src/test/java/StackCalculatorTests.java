import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.g19202.dmakogon.task2.calc.StackCalculator;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.logging.LogManager;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class StackCalculatorTests
{
    private StackCalculator stackCalc;

    @BeforeEach
    void turnOffLogging()
    {
        LogManager.getLogManager().reset();
    }

    @Test
    void executesSimpleProgram() throws IOException
    {
        String program = """
                DEFINE a 4
                PUSH a
                SQRT
                PRINT
                """;

        double expectedOutput = 2;

        testOutput(program, Double.toString(expectedOutput));
    }

    @AfterEach
    void finish()
    {
    }

    private void testOutput(String inputProgram, String expectedOutput) throws IOException
    {
        InputStream iStr = new ByteArrayInputStream(inputProgram.getBytes(StandardCharsets.UTF_8));
        ByteArrayOutputStream oStr = new ByteArrayOutputStream();
        PrintStream pStr = new PrintStream(oStr);

        stackCalc = new StackCalculator(iStr, pStr);

        stackCalc.execute();

        assertEquals(expectedOutput, oStr.toString().replace("\n", "").replace("\r", ""));
    }
}
