# P16 — Prompt Experiment Evaluator — Output First

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P16
- **Version:** 1.0.0
- **SDLC stage:** Cross-stage — prompt evaluation and learning
- **Derived from WBS 1.3 pair(s):** 4, 10
- **Learning Loop tag:** None

## Purpose
Evaluate before/after prompt experiments on executed output quality while treating prompt-design inspection as a separate diagnostic.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the same task/input, before prompt and output, after prompt and output, and rubric defined before scoring. The human score must be recorded before any AI self-score is consulted.

## Prompt template
```text
Compare the two executed AI responses for the same task/input.
Task/input: {{TASK_INPUT}}
Rubric defined before scoring: {{RUBRIC}}
Before prompt + output: {{BEFORE}}
After prompt + output: {{AFTER}}

First score the outputs against each rubric criterion and cite output evidence. Calculate delta. Classify primary result as Improved, No measurable improvement, or Regression. Separately assess prompt-design quality as a diagnostic. A better-looking prompt does not establish an output improvement. Note critical qualitative failures that numeric scoring may conceal. Do not self-assign human acceptance; record the evaluator's scores first.
```

## Example output
Output score: 3/6 → 6/6, delta +3; primary result Improved. Design check: PASS. Evidence: the after output includes missing boundary tests and ties each case to the acceptance list.

## Known limitation
Results depend on rubric quality, evaluator independence, and comparable inputs. Numeric scores are not objective facts by themselves.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
