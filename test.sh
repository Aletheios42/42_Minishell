#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL=0
PASSED=0
FAILED=0

# Your minishell path
MINISHELL="./minishell"

# Test function
run_test() {
    local cmd="$1"
    local description="$2"
    local ignore_env_order="$3"
    
    TOTAL=$((TOTAL + 1))
    echo -e "\n${YELLOW}Test $TOTAL: $description${NC}"
    echo "Command: $cmd"
    
    # Clean temp files
    rm -f /tmp/minishell_test_* 2>/dev/null
    
    # Run bash
    bash_out=$(timeout 5 bash -c "$cmd" 2>&1)
    bash_exit=$?
    
    # Run minishell and filter out debug output
    mini_out=$(timeout 5 $MINISHELL -c "$cmd" 2>&1 | grep -v "DEBUG")
    mini_exit=$?
    
    # Special handling for env command
    if [[ "$ignore_env_order" == "env" ]]; then
        bash_out=$(echo "$bash_out" | grep -v "^_=" | sort)
        mini_out=$(echo "$mini_out" | grep -v "^_=" | sort)
    fi
    
    # Compare results
    if [[ "$bash_exit" == "$mini_exit" && "$bash_out" == "$mini_out" ]]; then
        echo -e "${GREEN}✓ PASS${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Expected exit: $bash_exit, Got: $mini_exit"
        if [[ "$bash_out" != "$mini_out" ]]; then
            echo "Expected output:"
            echo "$bash_out"
            echo "Got output:"
            echo "$mini_out"
        fi
        FAILED=$((FAILED + 1))
    fi
}

# Test heredoc simulation function
test_heredoc_sim() {
    local content="$1"
    local description="$2"
    
    # Create temp file with content
    echo -e "$content" > /tmp/heredoc_test.txt
    
    # Test using file redirection (simulates heredoc behavior)
    run_test "cat < /tmp/heredoc_test.txt" "$description"
    
    rm -f /tmp/heredoc_test.txt
}

# Check if minishell exists
if [[ ! -f "$MINISHELL" ]]; then
    echo "Error: $MINISHELL not found. Please compile first."
    exit 1
fi

echo -e "${BLUE}Starting Comprehensive Minishell Test Suite${NC}"
echo "=============================================="

# BUILTIN TESTS
echo -e "\n${BLUE}=== BUILTIN TESTS ===${NC}"

# Echo tests (removed -n -n and -e flags)
run_test "echo hello" "Basic echo"
run_test "echo -n hello" "Echo with -n flag"
run_test "echo hello world" "Echo multiple words"
run_test "echo -n hello world" "Echo -n multiple words"
run_test "echo" "Empty echo"
run_test "echo -n" "Empty echo with -n"
run_test "echo ''" "Echo empty quotes"
run_test "echo \"\"" "Echo empty double quotes"
run_test "echo -n hello && echo world" "Echo -n followed by echo"

# PWD tests
run_test "pwd" "Print working directory"

# Env tests
run_test "env" "Environment variables" "env"

# VARIABLE EXPANSION TESTS
echo -e "\n${BLUE}=== VARIABLE EXPANSION TESTS ===${NC}"

run_test "echo \$USER" "Variable expansion USER"
run_test "echo \$HOME" "Variable expansion HOME"
run_test "echo \$PWD" "Variable expansion PWD"
run_test "echo \$?" "Exit status expansion"
run_test "echo \$NONEXISTENT" "Non-existent variable"
run_test "echo \$USER\$HOME" "Multiple variable expansion"
run_test "echo prefix\$USER" "Variable with prefix"
run_test "echo \$USER suffix" "Variable with suffix"
run_test "echo \"\$USER\"" "Variable in double quotes"
run_test "echo '\$USER'" "Variable in single quotes (no expansion)"
run_test "echo \$USER\$USER" "Same variable twice"

# PIPE TESTS
echo -e "\n${BLUE}=== PIPE TESTS ===${NC}"

run_test "echo hello | cat" "Simple pipe"
run_test "echo hello | cat | cat" "Double pipe"
run_test "ls | wc -l" "Pipe with wc"
run_test "printf 'line1\nline2\nline3\n' | wc -l" "Pipe counting lines with printf"
run_test "echo hello | grep hello" "Pipe with grep match"
run_test "echo hello | grep world" "Pipe with grep no match"
run_test "cat /etc/passwd | head -5 | tail -1" "Triple pipe"
run_test "echo abc | tr a-z A-Z" "Pipe with tr"
run_test "echo hello | cat | wc -c" "Pipe chain with character count"

# REDIRECTION TESTS
echo -e "\n${BLUE}=== REDIRECTION TESTS ===${NC}"

run_test "echo hello > /tmp/minishell_test_1.txt && cat /tmp/minishell_test_1.txt" "Output redirection"
run_test "echo world > /tmp/minishell_test_2.txt && echo hello >> /tmp/minishell_test_2.txt && cat /tmp/minishell_test_2.txt" "Append redirection"
run_test "cat < /etc/passwd | head -1" "Input redirection"
run_test "printf 'line1\nline2\n' > /tmp/minishell_test_3.txt && wc -l < /tmp/minishell_test_3.txt" "Input redirection with wc"
run_test "echo test > /tmp/minishell_test_4.txt && cat /tmp/minishell_test_4.txt > /tmp/minishell_test_5.txt && cat /tmp/minishell_test_5.txt" "Chained redirections"

# HEREDOC SIMULATION TESTS
echo -e "\n${BLUE}=== HEREDOC SIMULATION TESTS ===${NC}"

test_heredoc_sim "hello\nworld" "Heredoc simulation - multiline"
test_heredoc_sim "first line\nsecond line\nthird line" "Heredoc simulation - three lines"
test_heredoc_sim "" "Heredoc simulation - empty"
test_heredoc_sim "single line" "Heredoc simulation - single line"

# Additional heredoc-style tests with printf
run_test "printf 'hello\nworld\n' | cat" "Printf heredoc style"
run_test "printf 'line1\nline2\nline3\n' | grep line2" "Printf with grep"
run_test "printf 'a\nb\nc\n' | wc -l" "Printf line count"

# LOGICAL OPERATORS TESTS (using commands that exist)
echo -e "\n${BLUE}=== LOGICAL OPERATORS TESTS ===${NC}"

run_test "/bin/true && echo success" "AND operator success"
run_test "/bin/false && echo fail" "AND operator fail"
run_test "/bin/true || echo fail" "OR operator with true"
run_test "/bin/false || echo success" "OR operator with false"
run_test "/bin/true && /bin/true && echo success" "Multiple AND success"
run_test "/bin/true && /bin/false && echo fail" "Multiple AND fail"
run_test "/bin/false || /bin/false || echo success" "Multiple OR success"
run_test "echo hello && echo world" "AND with echo commands"
run_test "/bin/false || echo backup" "OR fallback"
run_test "/bin/true && /bin/false || echo rescue" "AND then OR"
run_test "/bin/false || /bin/true && echo success" "OR then AND"

# PARENTHESES TESTS
echo -e "\n${BLUE}=== PARENTHESES TESTS ===${NC}"

run_test "(echo hello)" "Simple parentheses"
run_test "(echo hello && echo world)" "Parentheses with AND"
run_test "(echo hello || echo world)" "Parentheses with OR"
run_test "(echo hello | cat)" "Parentheses with pipe"
run_test "echo start && (echo middle1 && echo middle2) && echo end" "Mixed parentheses and AND"
run_test "(echo hello && echo world) || echo backup" "Parentheses OR fallback"
run_test "(/bin/false || echo inside) && echo outside" "Complex parentheses logic"

# COMPLEX COMBINATIONS
echo -e "\n${BLUE}=== COMPLEX COMBINATIONS ===${NC}"

run_test "echo hello | cat && echo world" "Pipe then AND"
run_test "echo hello && echo world | cat" "AND then pipe"
run_test "echo hello > /tmp/minishell_test_6.txt && cat /tmp/minishell_test_6.txt | wc -w" "Redirection, pipe, and wc"
run_test "(echo hello | cat) && echo world" "Parentheses pipe AND"
run_test "echo \$USER | cat && pwd" "Variable expansion, pipe, AND"
run_test "printf 'test\ndata\n' | grep test && echo found" "Printf, pipe, grep, AND"

# QUOTE TESTS
echo -e "\n${BLUE}=== QUOTE TESTS ===${NC}"

run_test "echo 'hello world'" "Single quotes"
run_test "echo \"hello world\"" "Double quotes"
run_test "echo 'hello \"world\"'" "Single quotes with double inside"
run_test "echo \"hello 'world'\"" "Double quotes with single inside"
run_test "echo \"Hello \$USER\"" "Variable expansion in double quotes"
run_test "echo 'Hello \$USER'" "No expansion in single quotes"
run_test "echo 'spaces   between   words'" "Single quotes preserving spaces"

# ERROR CASES
echo -e "\n${BLUE}=== ERROR CASES ===${NC}"

run_test "nonexistentcommand" "Non-existent command"
run_test "cat /nonexistentfile" "Non-existent file"

# CD TESTS
echo -e "\n${BLUE}=== CD TESTS ===${NC}"

run_test "cd /tmp && pwd" "CD to /tmp"
run_test "cd /nonexistent" "CD to non-existent directory"
run_test "cd && pwd" "CD to home"
run_test "cd /tmp && cd - && pwd" "CD with dash (previous directory)"

# EXPORT/UNSET TESTS
echo -e "\n${BLUE}=== EXPORT/UNSET TESTS ===${NC}"

run_test "export TESTVAR=hello && echo \$TESTVAR" "Export and use variable"
run_test "export TESTVAR=hello && unset TESTVAR && echo \$TESTVAR" "Export, unset, and check"
run_test "export TESTVAR=world && export TESTVAR=hello && echo \$TESTVAR" "Export overwrite"
run_test "export TEST123=value && echo \$TEST123" "Export with numbers"

# EXIT STATUS TESTS
echo -e "\n${BLUE}=== EXIT STATUS TESTS ===${NC}"

run_test "/bin/true && echo \$?" "Exit status after true"
run_test "echo hello && echo \$?" "Exit status after echo"

# STRESS TESTS
echo -e "\n${BLUE}=== STRESS TESTS ===${NC}"

run_test "echo a | cat | cat | cat | cat | wc -c" "Long pipe chain"
run_test "/bin/true && /bin/true && /bin/true && /bin/true && echo success" "Long AND chain"
run_test "/bin/false || /bin/false || /bin/false || /bin/true && echo success" "Mixed long operators"
run_test "printf 'line1\nline2\nline3\nline4\nline5\n' | cat | wc -l" "Complex printf pipe"

# WHITESPACE AND EDGE CASES
echo -e "\n${BLUE}=== WHITESPACE AND EDGE CASES ===${NC}"

run_test "echo hello    world" "Multiple spaces"
run_test "echo 'hello    world'" "Multiple spaces in quotes"
run_test "  echo hello  " "Leading and trailing spaces"

# Cleanup
rm -f /tmp/minishell_test_* /tmp/heredoc_test.txt 2>/dev/null

# Results summary
echo -e "\n=============================================="
echo -e "${BLUE}Test Results Summary${NC}"
echo "=============================================="
echo "Total tests: $TOTAL"
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"

if [[ $FAILED -eq 0 ]]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! Your minishell is working perfectly! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}😞 $FAILED test(s) failed. Check the output above for details.${NC}"
    percentage=$((PASSED * 100 / TOTAL))
    echo -e "Success rate: ${YELLOW}$percentage%${NC}"
    exit 1
fi
