package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.core.config.Configurator;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandContext;
import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandExecutionContext;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.DefineAlreadyExistsException;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.EmptyStackException;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidParametersException;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.StackCalculatorException;

import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class CommandsTests
{
    private static final double DOUBLE_DELTA = 1e-10;

    @BeforeEach
    void turnOffLogging()
    {
        Configurator.setRootLevel(Level.OFF);
    }

    @Test
    void addAddsTwoNumbers() throws StackCalculatorException
    {
        // row format: op1, op2, expectedResult
        double[][] testValues = {
                {1, 2, 3},
                {0, 0, 0},
                {0.15, -1.12, 0.15 + (-1.12)},
                {Double.NaN, 1, Double.NaN},
                {Double.NaN, Double.NaN, Double.NaN},
                {Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY},
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY},
                {Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY},
                };

        executeBinaryWithValues(new Add(), testValues);
    }

    @Test
    void addAddsMultipleNumbers() throws StackCalculatorException
    {
        double[][] testValues = {
                {1, 2, 3, 4, 5, 0, 15},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0.12, -1.95, -2.2, 3, (0.12 - 1.95 - 2.2 + 3)},
                {Double.NaN, Double.NaN, Double.NaN, Double.NaN},
                {
                        Double.NEGATIVE_INFINITY,
                        Double.NEGATIVE_INFINITY,
                        Double.NEGATIVE_INFINITY,
                        Double.NEGATIVE_INFINITY
                },
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY},
                {
                        Double.POSITIVE_INFINITY,
                        Double.POSITIVE_INFINITY,
                        Double.POSITIVE_INFINITY,
                        Double.POSITIVE_INFINITY
                },
                };

        executeBinaryWithValues(new Add(), testValues);
    }

    @Test
    void addFailsWhenStackIsEmpty()
    {
        assertThrows(EmptyStackException.class, () -> executeCommandWithEmptyStack(new Add()));
    }

    @Test
    void subtractSubtractsTwoNumbers() throws StackCalculatorException
    {
        double[][] testValues = {
                {1, 2, 1 - 2},
                {0, 0, 0},
                {0.214, -1.12445, 0.214 - (-1.12445)},
                {0.214, -1.12445, 0.214 - (-1.12445)},
                {Double.NaN, 2.15, Double.NaN},
                {Double.NaN, Double.NaN, Double.NaN},
                {Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NaN},
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY},
                {Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.NaN},
                };

        executeBinaryWithValues(new Subtract(), testValues);
    }

    @Test
    void subtractSubtractsMultipleNumbers() throws StackCalculatorException
    {
        double[][] testValues = {
                // (1 - (2 - (3 - (4 - (5 - 0))))) = 3
                {1, 2, 3, 4, 5, 0, 3},
                {0, 0, 0, 0, 0, 0, 0},
                {0.65, -13, -212.35, -5, 0.65 - (-13 - (-212.35 - (-5)))},
                {Double.NaN, Double.NaN, Double.NaN, Double.NaN},
                {Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NaN},
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY},
                {Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.NaN},
                };
        executeBinaryWithValues(new Subtract(), testValues);
    }

    @Test
    void subtractFailsWhenStackIsEmpty()
    {
        assertThrows(EmptyStackException.class, () -> executeCommandWithEmptyStack(new Subtract()));
    }

    @Test
    void multiplyMultipliesTwoNumbers() throws StackCalculatorException
    {
        double[][] testValues = {
                {1, 3, 3},
                {0, 0, 0},
                {1, -1, -1},
                {5.12, 1.23, 5.12 * 1.23},
                {Double.NaN, 2.15, Double.NaN},
                {Double.NaN, Double.NaN, Double.NaN},
                {Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY},
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY},
                {Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY},
                };

        executeBinaryWithValues(new Multiply(), testValues);
    }

    @Test
    void multiplyMultipliesMultipleNumbers() throws StackCalculatorException
    {
        double[][] testValues = {
                // (1 - (2 - (3 - (4 - (5 - 0))))) = 3
                {1, 2, 3, 4, 5, 6, 720},
                {0, 0, 0, 0, 0, 0, 0},
                {1, 2, 3, 0, 0},
                {0.65, -13, -212.35, -5, (0.65 * -13 * -212.35 * -5)},
                {Double.NaN, Double.NaN, Double.NaN, Double.NaN},
                {
                        Double.NEGATIVE_INFINITY,
                        Double.NEGATIVE_INFINITY,
                        Double.NEGATIVE_INFINITY,
                        Double.NEGATIVE_INFINITY
                },
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY},
                {
                        Double.POSITIVE_INFINITY,
                        Double.POSITIVE_INFINITY,
                        Double.POSITIVE_INFINITY,
                        Double.POSITIVE_INFINITY
                },
                };

        executeBinaryWithValues(new Multiply(), testValues);
    }

    @Test
    void multiplyFailsWhenStackIsEmpty()
    {
        assertThrows(EmptyStackException.class, () -> executeCommandWithEmptyStack(new Multiply()));
    }

    @Test
    void divideDividesTwoNumbers() throws StackCalculatorException
    {
        double[][] testValues = {
                {1, 2, 0.5},
                {0, 1, 0},
                {1, -1, -1},
                {1.52, -215.15, 1.52 / -215.15},
                {Double.NaN, 2.15, Double.NaN},
                {Double.NaN, Double.NaN, Double.NaN},
                {Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NaN},
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY},
                {Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.NaN},
                };

        executeBinaryWithValues(new Divide(), testValues);
    }

    @Test
    void divideDividesMultipleNumbers() throws StackCalculatorException
    {
        double[][] testValues = {
                {1, 2, 3, 4, 5, 6, (1 / (2 / (3 / (4 / (5 / (double) 6)))))},
                {1, 1, 1, 1, 1},
                {0, 1, 2, 3, 0},
                {0.52, 1.123, 223.1, 3551.2555, (0.52 / (1.123 / (223.1 / 3551.2555)))},
                {Double.NaN, Double.NaN, Double.NaN, Double.NaN},
                {Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NaN},
                };

        executeBinaryWithValues(new Divide(), testValues);
    }

    @Test
    void divideFailsWhenDividingByZero()
    {
        double[][] testValues = {
                {Double.NEGATIVE_INFINITY, 1.1, Double.NEGATIVE_INFINITY, 0},
                {Double.POSITIVE_INFINITY, 7.25, Double.POSITIVE_INFINITY, 0},
                {1, 0, 0},
                };

        for (var row : testValues)
        {
            assertThrows(StackCalculatorException.class, () -> executeBinaryWithValues(new Divide(), row));
        }
    }

    @Test
    void divideFailsWhenStackIsEmpty()
    {
        assertThrows(EmptyStackException.class, () -> executeCommandWithEmptyStack(new Divide()));
    }

    @Test
    void sqrtCalculatesSquareRootCorrectly() throws StackCalculatorException
    {
        double[][] testValues = {
                {0, 0},
                {1, 1},
                {2, Math.sqrt(2)},
                {3, Math.sqrt(3)},
                {4, 2},
                {-1, Double.NaN},
                {-0, 0},
                {Double.NaN, Double.NaN},
                {Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY},
                {Double.NEGATIVE_INFINITY, Double.NaN},
                };

        executeUnaryWithValues(new Sqrt(), testValues);
    }

    @Test
    void sqrtFailsWhenStackIsEmpty()
    {
        assertThrows(EmptyStackException.class, () -> executeCommandWithEmptyStack(new Sqrt()));
    }

    @Test
    void defineDefinesValues() throws StackCalculatorException
    {
        Object[][] defines = {
                {"A", "10.5123"}, {"x", "0"}, {"y", "-1"}
        };
        Define defineCmd = new Define();
        CommandContext context = new CommandExecutionContext();
        for (var row : defines)
        {
            defineCmd.execute(context, row);
        }

        for (var row : defines)
        {
            assertEquals(context.getNamedParameter((String) row[0]), Double.parseDouble((String) row[1]));
        }
    }

    @Test
    void defineFailsWhenIncorrectValuesAreGiven()
    {
        Object[][] incorrectDefines = {
                {"A", "f112"}, {"x", ""}, {"a", " abc "},
                };
        Define define = new Define();
        CommandContext context = new CommandExecutionContext();
        for (var row : incorrectDefines)
        {
            assertThrows(InvalidParametersException.class, () -> define.execute(context, row));
        }
    }

    @Test
    void defineFailsWhenRedefining() throws StackCalculatorException
    {
        Define define = new Define();
        CommandContext context = new CommandExecutionContext();
        define.execute(context, new Object[]{"X", "10"});
        assertThrows(DefineAlreadyExistsException.class, () -> define.execute(context, new Object[]{"X", "12312.121"}));
    }

    @Test
    void popPopsValues() throws StackCalculatorException
    {
        CommandContext context = new CommandExecutionContext();
        context.push(1);
        context.push(20012.312d);
        context.push(Double.NaN);
        Pop pop = new Pop();
        for (int i = 0; i < 2; i++)
        {
            pop.execute(context, new Object[]{});
        }
        assertEquals(context.peek(), 1);
    }

    @Test
    void popFailsWhenStackIsEmpty()
    {
        assertThrows(EmptyStackException.class, () -> executeCommandWithEmptyStack(new Pop()));
    }

    @Test
    void pushPushesValues() throws StackCalculatorException
    {
        Object[] values = {"1", "21412.121", "-0", "-1.12"};
        Push push = new Push();
        CommandContext context = new CommandExecutionContext();
        for (var value : values)
        {
            push.execute(context, new Object[]{value});
            assertEquals(context.peek(), Double.parseDouble((String) value));
        }
    }

    @Test
    void pushPushesDefinedValues() throws StackCalculatorException
    {
        Object[][] defines = {
                {"X", "10"}, {"a", "12.3121"}
        };
        Push push = new Push();
        CommandContext context = new CommandExecutionContext();
        for (var define : defines)
        {
            context.addNamedParameter((String) define[0], Double.parseDouble((String) define[1]));
        }
        for (var define : defines)
        {
            push.execute(context, new Object[]{define[1]});
            assertEquals(context.peek(), Double.parseDouble((String) define[1]));
        }
    }

    @Test
    void pushFailsWhenIncorrectValuesAreGiven()
    {
        Object[] values = {"12.sada", "f", "X", ""};
        Push push = new Push();
        CommandContext context = new CommandExecutionContext();
        for (var ignored : values)
        {
            assertThrows(InvalidParametersException.class, () -> push.execute(context, new Object[]{values}));
        }
    }

    @Test
    void printPrintsValue() throws StackCalculatorException
    {
        double value = 10.123123d;
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        CommandContext context = new CommandExecutionContext(outputStream);
        context.push(value);
        new Print().execute(context, new Object[]{});
        assertEquals(value + String.format("%n"), outputStream.toString(StandardCharsets.UTF_8));
    }

    @Test
    void printFailsWhenStackIsEmpty()
    {
        assertThrows(EmptyStackException.class, () -> new Print().execute(new CommandExecutionContext(), new Object[]{}));
    }

    @Test
    void commentDoesntDoAnything()
    {
        double valueInStack = 112.1d;
        Object[] comments = {
                "COMMENT", "", "DEFINE x 5", "PUSH x", "+"
        };
        Comment comment = new Comment();
        CommandContext context = new CommandExecutionContext();
        context.push(valueInStack);
        for (var commentStr : comments)
        {
            comment.execute(context, new Object[]{commentStr});
        }
        assertEquals(context.peek(), valueInStack);
    }

    private void executeCommandWithNoParams(Command command, CommandExecutionContext context, int timesToExecute) throws StackCalculatorException
    {
        for (int i = 0; i < timesToExecute; i++)
        {
            command.execute(context, new Object[0]);
        }
    }

    private void executeCommandWithEmptyStack(Command command) throws StackCalculatorException
    {
        executeCommandWithNoParams(command, new CommandExecutionContext(), 1);
    }

    private void executeBinaryWithValues(Command command, double[] valuesRow) throws StackCalculatorException
    {
        testResultInStack(command, CommandParameters.FromArrayForBinary(valuesRow));
    }

    private void executeBinaryWithValues(Command command, double[][] testValues) throws StackCalculatorException
    {
        for (var row : testValues)
        {
            testResultInStack(command, CommandParameters.FromArrayForBinary(row));
        }
    }

    private void executeUnaryWithValues(Command command, double[][] testValues) throws StackCalculatorException
    {
        for (var row : testValues)
        {
            testResultInStack(command, CommandParameters.FromArrayForUnary(row));
        }
    }

    private void testResultInStack(Command command, CommandParameters params) throws StackCalculatorException
    {
        CommandExecutionContext context = new CommandExecutionContext();
        for (double val : params.stackOperands)
        {
            context.push(val);
        }

        executeCommandWithNoParams(command, context, params.timesToExecute);

        assertEquals(params.expectedResult, context.pop(), DOUBLE_DELTA);
    }

    private static class CommandParameters
    {
        public final double[] stackOperands;
        public final double expectedResult;
        public final int timesToExecute;

        public CommandParameters(double stackOperand, double expectedResult)
        {
            this(new double[]{stackOperand}, expectedResult);
        }

        public CommandParameters(double[] stackOperands, double expectedResult)
        {
            this(stackOperands, expectedResult, 1);
        }

        public CommandParameters(double stackOperand, double expectedResult, int timesToExecute)
        {
            this(new double[]{stackOperand}, expectedResult, timesToExecute);
        }

        public CommandParameters(double[] stackOperands, double expectedResult, int timesToExecute)
        {
            this.stackOperands = stackOperands;
            this.expectedResult = expectedResult;
            this.timesToExecute = timesToExecute;
        }

        public static CommandParameters FromArrayForBinary(double[] array)
        {
            int arrLen = array.length;
            return new CommandParameters(Arrays.copyOf(array, arrLen - 1), array[arrLen - 1], arrLen - 2);
        }

        public static CommandParameters FromArrayForUnary(double[] array)
        {
            int arrLen = array.length;
            return new CommandParameters(Arrays.copyOf(array, arrLen - 1), array[arrLen - 1], arrLen - 1);
        }
    }
}
