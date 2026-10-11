# P04 — Design Alternatives and Trade-off Review

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P04
- **Version:** 1.0.0
- **SDLC stage:** 3 — Design
- **Derived from WBS 1.3 pair(s):** 1, 3, 9
- **Learning Loop tag:** None

## Purpose
Compare plausible design options against stated constraints without overstating confidence or inventing non-functional requirements.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the approved requirement, constraints, architecture facts, known quality attributes, and decision criteria. Label desired quality attributes as unknown if not supplied.

## Prompt template
```text
Act as a software design reviewer. Based only on the inputs below, compare up to three feasible options.
Requirement: {{REQUIREMENT}}
Architecture/context: {{CONTEXT}}
Constraints: {{CONSTRAINTS}}
Decision criteria and priorities: {{CRITERIA}}

For each option, give: fit to requirement, complexity, compatibility risk, testability, operational impact, evidence/assumptions, and main downside. Do not introduce requirements that were not supplied; label assumptions. Recommend an option only if the criteria support it. State what evidence would change the recommendation.
```

## Example output
Option A is preferred only if it satisfies the existing public interface constraint; the input does not establish latency or memory targets, so those are left undecided.

## Known limitation
The model cannot assess production scale, measured performance, or team-specific constraints without evidence.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
