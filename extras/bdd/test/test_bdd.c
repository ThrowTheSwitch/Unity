/* ==========================================
 *  Unity Project - A Test Framework for C
 *  Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
 *  [Released under MIT License. Please refer to license.txt for details]
 * ========================================== */

#include "unity.h"
#include "unity_bdd.h"

void test_bdd_logic_test(void) {
    GIVEN("a valid statement is passed")
    {
        // Set up the context
        bool givenExecuted = true;

        WHEN("a statement is true")
        {
            // Perform the login action
            bool whenExecuted = true;
            
            THEN("we validate everything was worked")
            {
                // Check the expected outcome
                bool thenExecuted = true;

                TEST_ASSERT_TRUE(givenExecuted);
                TEST_ASSERT_TRUE(whenExecuted);
                TEST_ASSERT_TRUE(thenExecuted);
            }
        }
    }
} // end of case

void test_bdd_user_account(void) {
    GIVEN("a user's account with sufficient balance")
    {
        // Set up the context
        float accountBalance = 500.0;
        float withdrawalAmount = 200.0;

        WHEN("the user requests a withdrawal of $200")
        {
            // Perform the withdrawal action
            if (accountBalance >= withdrawalAmount)
            {
                accountBalance -= withdrawalAmount;
            } // end if
            THEN("the withdrawal amount should be deducted from the account balance")
            {
                // Check the expected outcome

                // Simulate the scenario
                float compareBalance = 500.0;
                TEST_ASSERT_LESS_THAN_FLOAT(accountBalance, compareBalance);
            }
        }
    }
} // end of case

void test_bdd_empty_cart(void) {
    GIVEN("a user with an empty shopping cart")
    {
        // Set up the context
        int cartItemCount = 0;

        WHEN("the user adds a product to the cart")
        {
            // Perform the action of adding a product

            THEN("the cart item count should increase by 1")
            {
                // Check the expected outcome
                cartItemCount++;

                TEST_ASSERT_EQUAL_INT(cartItemCount, 1);
            }
        }
    }
} // end of case

void test_bdd_valid_login(void) {
    GIVEN("a registered user with valid credentials")
    {
        // Set up the context
        const char* validUsername = "user123";
        const char* validPassword = "pass456";

        WHEN("the user provides correct username and password")
        {
            // Perform the action of user login
            const char* inputUsername = "user123";
            const char* inputPassword = "pass456";

            THEN("the login should be successful")
            {
                // Check the expected outcome
                // Simulate login validation
                TEST_ASSERT_EQUAL_STRING(inputUsername, validUsername);
                TEST_ASSERT_EQUAL_STRING(inputPassword, validPassword);
            }
        }

        WHEN("the user provides incorrect password")
        {
            // Perform the action of user login
            const char* inputUsername = "user123";
            const char* inputPassword = "wrongpass";

            THEN("the login should fail with an error message")
            {
                // Check the expected outcome
                // Simulate login validation
                TEST_ASSERT_EQUAL_STRING(inputUsername, validUsername);
                // TEST_ASSERT_NOT_EQUAL_STRING(inputPassword, validPassword);
            }
        }
    }
} // end of case

int main(void)
{
    UnityBegin("test_bdd.c");
    RUN_TEST(test_bdd_logic_test);
    RUN_TEST(test_bdd_user_account);
    RUN_TEST(test_bdd_empty_cart);
    RUN_TEST(test_bdd_valid_login);
    return UnityEnd();
}
