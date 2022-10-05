/*
 * @(#)MyLib.java    1.0, 04/24/98
 * 
 */

/**
 * The MyLib class is a direct port of mylib.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getline function returns a String type.
 *
 * @author Gary Gillespie 
 * @author Bryner Pancho (C ported to java)
 * @version 1.0, 04/24/98
 */

import java.io.*;      // System.in and System.out

class MyLibCharacter extends Base {
        private Character character;

        public MyLibCharacter (char ch) {
                character = new Character (ch);
        }

        public char charValue () {
                return character.charValue ();
        }

        public String toString () {
                return "" + character;
        }
}

public class MyLib extends Object {
     /**
      * A constant for passing in the baseout function.to display digits in hex.
      */
     static final int HEX = 16;

     /**
      * A constant for passing in the baseout function.to display digits 
      * in decimal.
      */
     static final int DECIMAL = 10;

     /**
      * A constant for converting ASCII characters to a numeric value.
      */
     static final long ASCII_ZERO = 48;

     private static final int CR = 13;      // Carriage Return (ASCII)
     private static final int MAX_LINE_LENGTH = 80; // Max str len for input.
     private static final char digits[] = 
         new String("0123456789abcdefghijklmnopqrstuvwxyz").toCharArray();

     private static Stack InStream = new Stack();

        /**
        * Reads in a decmial number from the input stream.
        *
        * @return  <code>long</code> 
        */
        public static long decin() 
                throws EOFException {

                char character;         // Character from user.
                long digit;             // Each digit as it is found.
                long sum = 0;

                try {
                        // Input terminates with a non-digit character.
                        character = getchar ();
                        while (Character.isDigit (character)) {

                                // Change from ASCII to numeric value.
                                digit = character & ~ASCII_ZERO;

                                // Accumulate number.
                                sum *= DECIMAL;
                                sum += digit;
                                character = getchar ();
                        }

                        // Return non-digit back to input stream.
                        ungetc (character);

                        return sum;   
                }

                catch (EOFException eof) {

                        // handle in caller
                        throw eof;
                }
        }



        /**
        * Takes in a positive or negative number and displays in a given
        * base to the screen.
        *
        * @param    Numeric positive or negative value to be displayed.
        * @param    Base to display number in.
        */
        public static void baseout (long number, long base) {

                if (number < 0) {
                        fputc('-', System.out);
                        posbaseout (-number, base);               
                }

                else {
                        posbaseout (number, base);
                }
        }


        /**
        * Takes in a positive number and base to be displayed to the screen.
        *
        * @param    A positive numeric value to be displayed.
        * @param    Base to display number in.
        */
        private static void posbaseout (long number, long base) {

                long quotient;                  // Quotient of div by 10.
                long remainder;                 //  Remainder of div by 10.

                // Remove one digit from number.
                //      digit (remainder) is for output,
                //      the rest of the number (quotient) 
                //      still needs to be printed.

                remainder = number % base;
                quotient = number / base;

                // Call function to output remaining digits, if any.
                if (quotient != 0)
                        posbaseout (quotient, base);

                // Output digit.
                fputc (digits [(int) remainder], System.out); 
        }


        /**
        * Takes in a positive number and displays it in hex.
        *
        * @param    A positive numeric value to be displayed in hex.
        */
        public static void hexout (long number) {

                // Output "0x" for hexidecimal.
                writeline ("0x", System.out);
                posbaseout (number, HEX);
        }


        /**
        * Clears the input stream buffer until the '\n' is
        *   encountered or has already been read indicated by
        *   the LastChar parameter..
        *
        * @param    Last character taken from the input stream.
        */
        public static void clrbuf (char LastChar) 
                throws EOFException {

                try {

                        // Check for buffer already being empty.
                        //   then remove all characters from the buffer
                        //   until empty.
                        while (LastChar != '\n')
                                LastChar = getchar ();
                }

                catch (EOFException eof) {
                        
                        // handle exception in caller
                        throw eof;
                }
                        
        }


        /**
        * Returns a character from the input stream.
        *
        * @return  <code>char</code> 
        */
        public static char getchar () 
                throws EOFException {

                char ToRet = '\0';

                // Check our local input stream first.
                //   If it's empty read from System.in
                if (InStream.isEmpty ()) {

                        try {
                                // Java likes giving the user the
                                // CR character too. Dumb, so just 
                                // ignore it and read the next character
                                // which should be the '\n'.                  
                                ToRet = (char) System.in.read ();
                                if (ToRet == CR)
                                        ToRet = (char) System.in.read ();
                                
                                // check for EOF
                                if ((long) ToRet == 0xFFFF)
                                        throw new EOFException ();
                        }

                        // Catch any errors in IO.
                        catch (EOFException eof) {

                                // Throw EOF back to caller to handle
                                throw eof;
                        }

                        catch (IOException ioe) {

                                writeline ("Unexpected IO Exception caught!\n",
                                                        System.out);
                                writeline (ioe.toString (), System.out);
                        }

                }

                // Else just pop it from the InStream.
                else
                        ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
                return ToRet;
        }


        /**
        * Displays a single character.
        *
        * @param    Character to display.
        */
        public static void fputc(char CharToDisp, PrintStream stream) {

                // Print a single character.
                stream.print (CharToDisp);   

                // Flush the system.out buffer, now. 
                stream.flush ();
        }


        /**
        * Prints out a newline character.
        *
        */
        public static void newline () {
                fputc ('\n', System.out);
        }


        /**
        * Prints out a string.
        *
        * @param    A string to print out.
        * @return     <code>long</code> The length of the string.
        */
        public static long writeline (String string, PrintStream stream) { 
                for (int index = 0; index < (string.length ()); index++)
                        fputc (string.charAt (index), stream);

                return string.length ();
        }


        /**
        * Returns a string from the user.
        *
        * @return     <code>String</code> 
        */
        public static String getline() 
                throws EOFException {

                char buffer[] = new char[MAX_LINE_LENGTH];          
                char input;           
                int index = 0; 

                try {
                        do {
                                input = getchar ();
                                buffer[index] = input;
                                index++;

                        } while (input != '\n' && (index < MAX_LINE_LENGTH-1));

                        return new String (buffer, 0, index - 1);              
                }

                catch (EOFException eof) {

                        // Throw EOF back to caller to handle
                        throw eof;
                }
        }


        public static String namein ()
                throws EOFException {

                char buffer[] = new char[MAX_LINE_LENGTH];          

                char character;         // input from user
                int index = 0;          // index into buffer

                try {
                        while (Character.isLetterOrDigit (character=getchar ())
                                || character == '_')
                                        buffer[index++] = character;
                        
                        /* put non-digit back into stdin */
                        ungetc (character);

                        return new String (buffer, 0, index);              
                }

                catch (EOFException eof) {

                        throw eof;
                }
        }

        /**
        * Places back a character into the input stream buffer.
        *
        * @param    A character to putback into the input buffer stream.
        */
        public static void ungetc (char ToPutBack) {

                // Push the char back on our local input stream buffer.
                InStream.push (new MyLibCharacter (ToPutBack));
        }
}
