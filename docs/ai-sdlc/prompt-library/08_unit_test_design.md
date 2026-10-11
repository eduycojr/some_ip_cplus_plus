# P08 — Unit Test Design from Acceptance Criteria

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P08
- **Version:** 1.0.0
- **SDLC stage:** 7 — Test design and verification
- **Derived from WBS 1.3 pair(s):** 3, 4, 10
- **Learning Loop tag:** None

## Purpose
Design non-tautological tests that map to acceptance criteria and include meaningful boundary cases.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the function/API signature, observed/required behavior, test framework, acceptance criteria, and relevant fixtures or constraints. Do not invent APIs or test framework methods.

## Prompt template
```text
Design unit tests for the supplied behavior.
Code/API: {{CODE_OR_SIGNATURE}}
Required behavior: {{BEHAVIOR}}
Acceptance criteria: {{ACCEPTANCE}}
Test framework: {{FRAMEWORK}}

First list test cases with purpose, input, expected observable result, and criterion covered. Include positive, negative, boundary, and regression cases when applicable. Then audit your candidate list against every acceptance criterion and identify gaps before producing example test code. Avoid tautological assertions that merely repeat the implementation. Flag any framework syntax you cannot verify.
```

## Example output
Case: input without delimiter; expected: parser returns failure; criterion: malformed input must not be accepted. Additional case: all-whitespace input if the requirement defines its handling.

## Known limitation
Coverage percentage alone does not establish test quality. Real compilation and test execution are required.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
