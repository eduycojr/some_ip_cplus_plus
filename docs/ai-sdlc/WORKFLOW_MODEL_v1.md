# WBS 2.1 — AI-assisted SDLC Workflow Model v1

**Status:** Training/simulation deliverable for pilot review. Not yet installed in the user's remote repository, live-verified against the user's Copilot account, or peer-approved. **Version:** `v1.0.0-draft`.

## Purpose

Formalize the existing AI-assisted engineering method into an 11-stage repeatable workflow for an existing GitHub repository, using **GitHub Copilot Free via the GitHub.com web interface** as the AI interaction surface. The repository remains the versioned source of truth. This is a **repository-instruction and manual-orchestration adapter**, not a plugin, API integration, automatic prompt chain, or claim that Copilot Chat can run a local toolchain.

## The 11 stages

| # | Stage | Entry inputs | AI role | Human/engineering role | Required output / handover | Exit / stop condition | Prompt library |
|---:|---|---|---|---|---|---|---|
| 1 | Requirement | Ticket/problem, observed/expected behavior, stakeholders, constraints, non-goals | Clarify ambiguity; propose acceptance criteria; expose unknowns | Confirm intended behavior, severity, scope, and observable acceptance | Approved requirement record, acceptance criteria, non-goals, questions | STOP if intended behavior or required acceptance is materially ambiguous | P01; P15 when checking claims |
| 2 | Decomposition | Approved requirement and criteria | Split into atomic tasks; map each to outputs/dependencies/tests | Validate ordering, ownership, sizing, and traceability | Task tree with dependencies and criterion IDs | STOP if dependency, task boundary, or acceptance mapping is unclear | P02 |
| 3 | Repository exploration | Task tree, selected repository, relevant paths and code context | Locate probable call paths/modules/tests/build files and cite evidence | Inspect actual repository files, confirm entry points and boundaries | Evidence-based repo map, paths/symbols, test/build discovery, unknowns | STOP before planning implementation if key behavior/path is unverified | P03; P15 |
| 4 | Plan | Approved requirement, decomposition, repo findings, constraints, existing tests/toolchain | Propose minimum ordered implementation/test steps and alternatives where useful | Check feasibility, compatibility, estimates, dependencies, rollback | Sequenced implementation plan with outputs, commands to run, and done conditions | STOP if a required input/toolchain is unknown or plan conflicts with constraints | P05; P04 if material design options; P17 for multi-turn gate |
| 5 | Plan critique | Proposed plan, acceptance criteria, repo evidence, constraints | Adversarially challenge assumptions, edge cases, omissions, test gaps, security, regression, performance, rollback, observability, and scope | Decide which findings are valid; amend or approve the plan | Critique findings severity-tagged; accepted/rejected findings with reasons; revised plan | STOP until Critical/High findings resolved or explicitly accepted by authorized human; no implementation during critique | Adapter-only stage prompt `stage-prompts/05_plan_critique.md`; P05 as input, P10/P11 as conditional checks |
| 6 | Implement | Critiqued, approved plan; exact relevant source; scoped acceptance | Propose minimal diff; explain each hunk and risks | Apply/inspect patch in an authorized branch; understand code; keep scope narrow | Reviewable diff, file list, rationale, changed behavior, risk notes | STOP if actual source/API differs from assumptions, scope expands, or generated code cannot be explained | P06; P18 if a valid tool payload is required |
| 7 | Test generation | Confirmed behavior, code/API, acceptance criteria, framework and existing tests | Create behavior-focused positive/negative/boundary/regression test proposals | Reject tests that merely restate implementation; add missing cases; run in actual environment | Test cases tied to acceptance criteria and test source changes, if authorized | STOP if test framework or expected behavior is unknown; generated tests are not proof they pass | P08 |
| 8 | Debug | Actual failure reproduction, command, environment, logs, expected/actual, code/diff | Generate falsifiable hypotheses; suggest next discriminating experiment; analyze test failures | Reproduce, minimize, instrument, isolate, fix, re-run; preserve logs | Root cause supported by evidence or unresolved hypotheses; fix and regression-test evidence | STOP if failure isn't reproducible and evidence is insufficient; never label hypothesis as root cause without support | P07; P09 |
| 9 | Human review | Actual diff, approved requirement, design rationale, test evidence, applicable policy | Find defects, API misuse, silent behavior change, complexity, security/licensing and maintainability risks | Perform independent code review and make/record merge/acceptance decision | Findings by severity, disposition, reviewer identity/decision, unresolved risk | STOP for unresolved blocker/high-risk finding, policy uncertainty, or no independent review where required | P10; P11 when security/data/policy scope applies |
| 10 | Verify | Final diff/commit candidate, acceptance criteria, exact test/build/static-analysis logs | Map every criterion to available evidence; validate structured outputs if relevant; classify unsupported claims | Execute required checks locally or in CI; inspect output and final diff; bind evidence to exact revision | Verification matrix: criterion → command/CI job → result → evidence link → SHA; exceptions | STOP if any required check is missing/failed, evidence is for another revision, or only AI assertion exists | P14 for schema; P15 for claims; P16 only for controlled prompt comparison |
| 11 | Document / maintain | Verified behavior, actual final diff/SHA, test results, review decisions, known limits | Draft docs, release/change notes, handover and maintenance tasks from verified facts | Validate every claim, publish through repository process, note follow-ups, measure/reuse learning loop | Updated documentation/change notes, known limitations, next maintenance tasks, learning-loop record | STOP release/closure if docs contradict code, verification or reviewer decision | P12; P13 for refactoring/controlled maintenance |

## Cross-stage rules

1. **Evidence is separate from model output.** “Copilot proposed test X” does not mean “test X passed.” A plan is not an execution result; a diff response is not a repository commit.
2. **Handover record required.** Each stage records: task/WBS ID; stage/prompt IDs; input sources and revision; facts/inferences/unknowns; output; actual verification performed; open issues; human decision; next-stage inputs.
3. **Prompt selection is conditional.** Use the smallest suitable subset of the 18 specialist prompts. The 11 workflow stages are the required control flow; they do not imply 18 prompts must all run.
4. **Stage 5 plan critique is a deliberate gap-fill.** The WBS 1.4 library's original working map uses its own 11 categories, which do not match the exact 11 stages stated in WBS 2.1. The existing prompt set has no dedicated plan-critique prompt; this adapter adds one. P16 evaluates before/after prompt outputs and is not a substitute for plan critique.
5. **Human stop conditions are mandatory.** Human confirmation is necessary for requirements, intended behavior, security/policy decisions, plan acceptance, application of risky changes, code review, and completion acceptance.
6. **GitHub web capability boundary.** Copilot Chat may use repository context and repository custom instructions, but do not assume it has a shell, can run the project's tests, or can write/commit/push in the selected interface. Use observed capability only. Actual build/test proof must come from executed commands/CI logs.
7. **Free tier optimization.** Keep one task per conversation where practical; use structured multi-stage requests for analysis-only stages; do not spend interactions repeatedly rediscovering the repo; split into a new chat only with the handover record. Confirm current usage in GitHub because quota/credits may change.

## Suggested handover statuses

`DRAFT` → `WAITING_FOR_INPUT` / `PROPOSED` → `HUMAN_APPROVED` → `IMPLEMENTED` → `VERIFIED` → `REVIEWED` → `ACCEPTED`.
Use `BLOCKED` whenever a required input/decision/evidence is missing. Status names are process conventions, not claims that the UI enforces them.

## Acceptance checklist for WBS 2.1

- [ ] Each of the 11 WBS 2.1 stages has input, output, AI role, human role, and stop/exit condition.
- [ ] Existing WBS 1.4 prompt IDs are mapped without forcing all 18 prompts on every task.
- [ ] Plan critique gap is covered by a separate adapter prompt.
- [ ] GitHub.com Copilot Free capability limitations are documented.
- [ ] Repository instructions are installed and confirmed to load in actual repo chat.
- [ ] A safe, small pilot is run using actual repository context and real verification evidence.
- [ ] Peer review completed and version tag `v1.0.0` created only after approval.

## GitHub official references

- Copilot web custom instructions: https://docs.github.com/en/copilot/concepts/prompting/response-customization
- Support matrix for instructions / prompt files: https://docs.github.com/en/copilot/reference/custom-instructions-support
- Individual plan / Free usage: https://docs.github.com/en/copilot/concepts/billing/individual-plans
- Repository instructions how-to: https://docs.github.com/en/copilot/how-tos/copilot-on-github/customize-copilot/add-custom-instructions/add-repository-instructions
