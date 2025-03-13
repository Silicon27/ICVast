import subprocess

# TODO: make it visually appealing and add more tests
def test_merge():
    # Run merge script
    test = subprocess.run(["cvast", "cvFiles/merge_test.cv"])
    # Check the exit code
    assert test.returncode == 0

    # Run fn script
    test = subprocess.run(["cvast", "cvFiles/fn_test.cv"])

test_merge()

