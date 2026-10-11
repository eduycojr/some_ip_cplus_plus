# P14 — Validate Structured Tool Output

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P14
- **Version:** 1.0.0
- **SDLC stage:** 11 — Tool and automation integration
- **Derived from WBS 1.3 pair(s):** 3, 12
- **Learning Loop tag:** W1.3-LL-02

## Purpose
Validate a candidate structured output against an explicitly supplied schema without inferring missing field names or identifiers.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the full schema, required and optional fields, value types, permitted enum values, constraints, and candidate output. If the schema is partial, mark validation as blocked rather than inventing a contract.

## Prompt template
```text
Validate the candidate output against the exact contract below.
Schema/contract: {{EXACT_SCHEMA}}
Required fields: {{REQUIRED_FIELDS}}
Optional fields: {{OPTIONAL_FIELDS}}
Enums/constraints: {{ENUMS_CONSTRAINTS}}
Candidate: {{CANDIDATE}}

Return: VALID or INVALID/BLOCKED; each field checked; missing/extra fields; type/enum errors; invented or unsupported values; corrected candidate only when the schema is complete enough to determine it. If the schema is partial, do not fill gaps by guessing. Include a machine-checkable validation approach when possible.
```

## Example output
Status: BLOCKED — the supplied contract does not define the required repository identifier field. No corrected JSON is emitted until the exact schema is provided.

## Known limitation
A textual validation is not equivalent to running a schema validator. Use a real validator for machine-enforced constraints.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
