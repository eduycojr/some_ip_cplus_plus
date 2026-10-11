# P18 — Tool Payload Generation with Exact Schema Gate

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P18
- **Version:** 1.0.0
- **SDLC stage:** 11 — Tool and automation integration
- **Derived from WBS 1.3 pair(s):** 12
- **Learning Loop tag:** W1.3-LL-02

## Purpose
Apply the WBS 1.3 learning-loop change: generate a tool payload only when its exact schema and required values are available.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the full tool schema/contract, allowed values, required inputs and trusted identifiers. When any required schema detail or identifier is missing, provide the missing-information list instead of a guessed payload.

## Prompt template
```text
Generate a payload for this tool only if the contract is complete.
Tool purpose: {{TOOL_PURPOSE}}
Exact schema: {{SCHEMA}}
Required inputs and trusted identifiers: {{INPUTS}}
Example valid payload if available: {{VALID_EXAMPLE}}

Preflight: list required fields and allowed values. If schema, required field, or trusted identifier is missing, return BLOCKED plus the exact information needed; do not emit a guessed payload. Otherwise emit the payload in the requested format only, then validate required fields, types, enums, and extra fields against the supplied schema. Do not invent IDs, fields, enum values, or success claims.
```

## Example output
BLOCKED — the contract omits the required repository ID format and allowed operations. Request those two schema details; do not emit JSON yet.

## Known limitation
The prompt cannot guarantee tool acceptance. The receiving tool/API contract and a real validator remain authoritative; LL-02 remains pending until retest/measure/reuse.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
