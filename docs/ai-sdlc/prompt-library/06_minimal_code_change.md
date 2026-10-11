# P06 — Minimal, Scope-Controlled Code Change

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P06
- **Version:** 1.0.0
- **SDLC stage:** 5 — Implementation
- **Derived from WBS 1.3 pair(s):** 1, 5, 8
- **Learning Loop tag:** None

## Purpose
Generate or review a narrowly scoped candidate patch tied to a confirmed defect or requirement.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the exact relevant source, the required behavior, reproduction or acceptance criteria, interface constraints, and permitted edit scope. Identify language/version if known.

## Prompt template
```text
Propose the smallest code change that satisfies the confirmed requirement.
Relevant code: {{CODE}}
Required behavior/defect: {{BEHAVIOR}}
Acceptance tests: {{ACCEPTANCE}}
Constraints: {{CONSTRAINTS}}

Before presenting a patch, state the root-cause evidence and affected behavior. Change only defect/requirement-relevant code. Preserve public interfaces and existing behavior outside scope. Do not invent symbols or libraries. Provide a unified diff if possible, explain each changed hunk in one line, and list tests needed. Mark any assumption as unverified. Do not claim compilation or test success unless execution output is supplied.
```

## Example output
Hunk rationale: “Check for missing delimiter before slicing the key/value fields, preventing a not-found index from being used.” Verification still required: malformed-line test and existing parser tests.

## Known limitation
Generated code may not compile against unseen types, build flags, or platform constraints. A human must inspect and run tests.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
