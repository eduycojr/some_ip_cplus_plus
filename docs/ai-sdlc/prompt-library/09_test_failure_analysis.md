# P09 — Test Failure Triage and Regression Mapping

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P09
- **Version:** 1.0.0
- **SDLC stage:** 7 — Test execution and defect correction
- **Derived from WBS 1.3 pair(s):** 3, 8, 10
- **Learning Loop tag:** None

## Purpose
Convert a failed test run into a focused, evidence-backed next action without fabricating root cause.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Supply exact test command, stdout/stderr, failing test names, expected/actual behavior, environment, and relevant code/diff. Distinguish collected output from assumptions.

## Prompt template
```text
Analyze this test failure using only the supplied run evidence.
Command/environment: {{COMMAND_ENV}}
Failure output: {{FAILURE_OUTPUT}}
Expected vs actual: {{EXPECTED_ACTUAL}}
Relevant diff/code: {{DIFF_CODE}}

Return: confirmed facts, likely causes ranked by evidence, missing evidence, smallest next diagnostic step, regression risk, and the exact re-run needed. Do not say tests passed unless the provided output shows a successful run. Keep the failure distinct from a suspected cause.
```

## Example output
Confirmed: `test_parse_missing_delimiter` failed because expected `false` but got `true`. Unknown: whether the failure comes from the parser or fixture until relevant code and test data are inspected.

## Known limitation
The analysis is only as good as the supplied run output and may miss environment-specific effects.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
