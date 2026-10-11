# Session Start Prompt — GitHub.com Copilot Chat

Copy this into a new Copilot Chat conversation after selecting the intended repository context. Replace bracketed values.

```text
Task/WBS: [WBS ID and task statement]
Current stage(s): [stage number/name(s)]
Branch/commit, if known: [branch and SHA]
Requested scope: [read-only analysis / plan / patch proposal / review / verification mapping]

Use the repository's `.github/copilot-instructions.md`, `docs/ai-sdlc/WORKFLOW_MODEL_v1.md`, `docs/ai-sdlc/PROMPT_ROUTER.md`, and relevant specialist prompt(s) under `docs/ai-sdlc/prompt-library/prompts/`.

First state whether you can actually access each named file in the current repository context. If not, ask me to paste it; do not infer its content. Identify the smallest applicable prompt set and the required inputs.

For every stage you execute, return:
1. Stage number/name and prompt ID(s)
2. Inputs/evidence inspected (repository paths, issue links, excerpts, commit SHA)
3. Facts vs inferences vs unknowns
4. Output/artifact or proposal
5. Verification still required and who must perform it
6. Human gate/stop condition and current status
7. Concise handover for the next stage

Do not claim tests/builds/CI, repository writes, commits, pushes, PRs, reviewer acceptance, or company-policy approval unless real evidence is present in this conversation. Use BLOCKED for missing required context. Do not implement before Stage 5 plan critique has been resolved and I explicitly approve implementation.

For this request, process only these stages: [stage selection]. Do not run the entire 11-stage workflow unless asked.
```
