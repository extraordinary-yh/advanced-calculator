import java.io.*;

/* DO NOT CHANGE:  This file is used in evaluation */

class Operator extends Base {
        public char ascii;              /* ascii character of operator */
        public int index;               /* index in parallel arrays */
        public int priority;            /* priority of operator */

        private static final String operators = "= ()+-*/^ !";

        public Operator (char ascii_code) {
        
                index = 0;      /* index of the operand in character array */
                ascii = ascii_code;

                /* compute function index */
                while (operators.charAt (index) != ascii)
                        index++;

                priority = index >> 1;
        }

        public String toString () {
                return "" + ascii;
        }

	public String getName () {
		return toString ();
	}
}

class Variable extends Base {
        public String name;             /* name of variable */
        public long value;              /* value of interest */

        public Variable (String nm, long val) {
                name = new String (nm);
                value = val;
        }
        public Variable (Variable variable) {
                name = new String (variable.name);
                value = variable.value;
        }

        public String getName () {
                return name;
        }
		public boolean equals (Object object) {
			if (this == object)
				return true;

			if (!(object instanceof Variable))
				return false;
		
			Variable otherVar = (Variable) object;
		
			return name.equals (otherVar.getName ());
		}
        public boolean isGreaterThan (Base base) {
                return (name.compareTo (base.getName ()) > 0) ? true : false;
        }
        public String toString () {
                return name + "(" + value + ")";
        }

        public Variable assignValue (long val) {
                
                Variable retval;        // return value

                // give variable its value
                value = val;
                retval = new Variable (this);

                return retval;
        }
}

/* declare the word to place on calulator stacks */
class Word extends Base {
        
        private static final int OPERATOR = 0;
        private static final int VALUE = 1;
        private static final int VARIABLE = 2;

        public Operator op;
        public long value;
        public Variable var;

        public int type;

        public Word (char character) {
                op = new Operator (character);
                type = OPERATOR;
        }
        public Word (long val) {
                value = val;
                type = VALUE;
        }
        public Word (String name, long val) {
                var = new Variable (name, val);
                type = VARIABLE;
        }
        public Word (String name) {
                var = new Variable (name, 0);
                type = VARIABLE;
        }

        public boolean isOperator () {
                return (type == OPERATOR);
        }
        public boolean isValue () {
                return (type == VALUE);
        }
        public boolean isVariable () {
                return (type == VARIABLE);
        }
        public String toString () {

                String temp = "";

                switch (type) {
                case OPERATOR:
                        temp += op + " ";
                        break;
                case VALUE:
                        temp += value + " ";
                        break;
                case VARIABLE:
                        temp += var + " ";
                        break;
                }

                return temp;
        }               

	public String getName () {

		return "Word";
	}
}

public class Calculator {

        private SymTab<Variable> symtab;
        private Stack<Word> stack1, stack2;

        public Calculator () {
                symtab = new SymTab<Variable> ("CalcTree");
                stack1 = new Stack<Word> ();
                stack2 = new Stack<Word> ();
        }

        public String writePostfix() {
                return "" + stack1;
        }


        public String toString () {

                String temp = "Stack 1 is:  " + stack1;
                temp += "\nStack 2 is:  " + stack2;
                temp += "\nSymbol Table is:\n" + symtab;

                return temp;
        }

        public long evaluate () {
                Word op1, op2 = null;           /* the operands */
                long operand1, operand2;        /* the operands as long */
                Word operation;         /* the operation being performed */
                long result = 0;                /* result of computation */

                /* reverse stack1 onto stack2 */
                while (!stack1.isEmpty ())
                        stack2.push (stack1.pop ());

                while (!stack2.isEmpty ()) {


                        /* numbers and variables go to stack1 */
                        if (!((Word) stack2.top ()).isOperator ())
                                stack1.push (stack2.pop ());
                        
                        /* operand found */
                        else {
                                operation = (Word) stack2.pop ();
                                
                                /* get operands */
                                op1 = (Word) stack1.pop ();
                                operand1 = (op1.isVariable ())
                                ? ((Variable) symtab.lookup (op1.var)).value
                                        : op1.value;

                                if (operation.op.ascii != '!') {
                                        op2 = (Word) stack1.pop ();
                                        if (op2.isVariable ()
                                        && (operation.op.ascii != '='))
                                                operand2 = ((Variable)
                                                symtab.lookup (op2.var)).value;
                                        else
                                                operand2 = op2.value;
                                }
                                else
                                        operand2 = operand1;

                                /* evaluate expression and push to stack1 */
                                if (operation.op.ascii != '=')

                                        switch (operation.op.ascii) {

                                        case '+':
                                                result = add
                                                        (operand1, operand2);
                                                break;

                                        case '-':
                                                result = sub
                                                        (operand1, operand2);
                                                break;

                                        case '*':
                                                result = mult
                                                        (operand1, operand2);
                                                break;

                                        case '/':
                                                result = div
                                                        (operand1, operand2);
                                                break;

                                        case '^':
                                                result = exp
                                                        (operand1, operand2);
                                                break;

                                        case '!':
                                                result = fact
                                                        (operand1, operand2);
                                                break;
                                        }

                                // variable assignment
                                else {
                                        Variable svar = 
                                                op2.var.assignValue (operand1);
                                        symtab.insert (svar);
                                        result = op2.var.value;
                                }

                                stack1.push (new Word (result));
                        }
                }

                op1 = (Word) stack1.pop ();
                result = (op1.isVariable ())
                        ? ((Variable) symtab.lookup (op1.var)).value
                        : op1.value;

                return result;
        }


        public long convertInToPost () 
                throws EOFException {
                char character;         /* character input from the user */
                Word word;              /* a word for the stack */
                
                try {
                while ((character = MyLib.getchar ()) != '\n') {

                        /* ignore blanks */
                        if (Character.isSpaceChar (character))
                                continue;
                        
                        if (Character.isDigit (character) || character == '.') {

                                /* for decin call */
                                MyLib.ungetc (character);

                                /* place any numbers onto stack1 */

                                stack1.push (new Word (MyLib.decin()));
                        }

                        else if (Character.isLetter (character)) {

                                String variable_name;

                                /* for namein call */
                                MyLib.ungetc (character);

                                /* place any numbers onto stack1 */

                                variable_name = MyLib.namein ();

                                stack1.push (new Word (variable_name));
                        }

                        else if (character == '(') {

                                /* open parenthesis go onto stack2 */

                                stack2.push (new Word ('('));
                        
                        }
                        else if (character == ')') {

                                /* pop stack2 to stack1 until matching paren */
                                word = (Word) stack2.pop ();
                                while (word.op.ascii != '(') {

                                        stack1.push (word);
                                        word = (Word) stack2.pop ();
                                }
                        }

                        /* operator must be encountered */
                        else {
                                word = new Word (character);/*store on stack */

                                /* pop stack2 to stack1 til empty/lower prio */
                                while (!stack2.isEmpty () &&
                                        ((Word) stack2.top ()).op.priority 
                                        >= word.op.priority)
                                                stack1.push (stack2.pop ());

                                /* finally place operator on stack2 */
                                stack2.push (word);
                        }
                }
                }

                catch (EOFException eof) {

                        throw eof;
                }

                /* place any remaining stack2 items to stack1 ==> done */
                while (!stack2.isEmpty ())
                        stack1.push (stack2.pop ());
                
                return 1;
        }


        static private long add (long addendx, long addendy) {
                return addendx + addendy;
        }

        static private long div (long divisor, long dividend) {
                return dividend / divisor;
        }

        static private long exp (long power, long base) {
                long retval = 1;

                while (power > 0) {
                        retval *= base;
                        power--;
                }
                
                while (power < 0) {
                        retval /= base;
                        power++;
                }

                return retval;
        }

        static private long fact (long number, long ignored) {
                return (number <= 1) ? 1 : number * fact (number - 1, ignored);
        }

        static private long mult (long factorx, long factory) {
                return factorx * factory;
        }

        static private long sub (long subtrahend, long minuend) {
                return minuend - subtrahend;
        }
}
