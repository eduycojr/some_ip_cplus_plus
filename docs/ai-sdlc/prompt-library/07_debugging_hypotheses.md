# P07 — Debugging Hypotheses and Falsification Tests

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P07
- **Version:** 1.0.0
- **SDLC stage:** 6 — Debugging and defect isolation
- **Derived from WBS 1.3 pair(s):** 6, 9, 10
- **Learning Loop tag:** None

## Purpose
Produce ranked, testable defect hypotheses instead of asserting a cause from incomplete evidence.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Supply error/stack trace, relevant source, reproduction steps, environment/toolchain, and recent changes when available. Redact secrets and personal data.

## Prompt template
```text
Diagnose the failure without treating the first plausible explanation as proven.
Observed failure: {{FAILURE}}
Stack/logs: {{LOGS}}
Relevant code: {{CODE}}
Reproduction/environment: {{ENVIRONMENT}}

Return ranked hypotheses with supporting evidence, contradicting evidence, confidence (low/medium/high), and one falsification test per hypothesis. State the smallest next experiment that distinguishes the top hypotheses. Do not claim a race, memory error, or platform issue without evidence. Do not propose a broad refactor as the first response.
```

## Example output
Hypothesis 1: invalid pointer/lifetime issue (medium); Hypothesis 2: race (low); falsification: repeat under the relevant sanitizer or controlled synchronization test if available.

## Known limitation
Stack traces and logs may omit the root cause. Confidence labels are qualitative and do not substitute for a reproducible test.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
