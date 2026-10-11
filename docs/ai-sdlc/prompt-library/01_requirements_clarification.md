# P01 — Requirements Clarification

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P01
- **Version:** 1.0.0
- **SDLC stage:** 1 — Requirements and acceptance
- **Derived from WBS 1.3 pair(s):** 1, 5, 9
- **Learning Loop tag:** None

## Purpose
Turn an incomplete feature request into explicit, testable requirements without silently inventing product behavior.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide: (a) request/problem statement, (b) known context, (c) constraints, (d) stakeholder or user when known, and (e) any known non-goals. Mark unavailable information as unknown. Do not include secrets or personal/customer data.

## Prompt template
```text
You are a requirements analyst for a software engineering task.

Request: {{REQUEST}}
Known context: {{CONTEXT}}
Constraints: {{CONSTRAINTS}}
Known non-goals: {{NON_GOALS}}

Produce: 1) confirmed facts, 2) assumptions, 3) open questions ranked by impact, 4) functional requirements, 5) non-functional requirements when supported, 6) acceptance criteria written as observable Given/When/Then checks, and 7) an out-of-scope list. Do not turn an assumption into a fact. If a requirement cannot be determined from the inputs, label it OPEN rather than guessing.
```

## Example output
Input asks for a config parser to reject malformed key/value lines. Example output includes: “AC-01: Given a line with no `=`, when parsing is attempted, then parsing returns failure and reports the line as invalid.” Any behavior not specified by the request is marked OPEN.

## Known limitation
A well-structured requirement set cannot replace product-owner decisions. Acceptance criteria remain proposed until a human confirms intended behavior.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
