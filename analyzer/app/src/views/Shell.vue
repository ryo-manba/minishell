<template lang="pug">
.self
  .prompt
    .line
      textarea(
        v-model="viewData.line_buffer"
        autocapitalize=none
        autocomplete=off
        @keydown.enter="eventHandlers.readline"
      )
    .present
      code {{ present_line_fordisp }}
  .lexer
    h3 Lexed: Word List
    .flattened_list(v-if="lexed_flattened_wordlist")
      .word(v-for="word in lexed_flattened_wordlist")
        .start_at start_at: {{ word.i }}
        .lex_type(:class="word.lex_type") {{ word.lex_type }}
        .body
          code {{ word.word }}
  .parser
    h3 Parsed: PipelineList
    .state(v-if="parsed_state")
      h4.parse_finished(v-if="parsed_state.finished") Finished
      h4.parse_error(v-else-if="parsed_state.parse_error")
        | Parse Error: {{ parsed_state.parse_error }}
        template(
          v-if="parsed_state.error_location_token"
        ) 
          | (at token {{ parsed_state.error_location_token.lex_type }}
          code {{ parsed_state.error_location_token.word }}
          | )
      h4.parse_unexpexted(v-else) Unexpected State
      h5 PipelineList
      .list(v-if="parsed_state.flattened_pipelinelist")
        .andor_list(
          v-for="andor in parsed_state.flattened_pipelinelist.andors"
        )
          .head
            h5 AND-OR list
            .joint(v-if="andor.andor.joint")
              | term:
              code {{ andor.andor.joint }}
          .pipeline(
            v-for="pipeline in andor.pipelines"
          )
            .head
              h5 Pipeline
              .joint(v-if="pipeline.pipeline.joint")
                | term:
                code {{ pipeline.pipeline.joint }}
            .clause(
              v-for="clause in pipeline.clauses"
            )
              h5 Clause
              h5 Redirections
              .redirs(
                v-if="clause.redirs && clause.redirs.length > 0"
              )
                .redir(
                  v-for="redir in clause.redirs"
                )
                  .operator
                    | {{ get_desc_redir_op(redir) }}
                  | fd:
                  code {{ get_redir_fd(redir).fd }}
                  span(
                    v-if="get_redir_fd(redir).default"
                  ) (default)
                  |, target:
                  code(
                    v-if="redir.operand_right"
                  ) {{ redir.operand_right.word }}
              h5 Command Tokens
              .strees
                .stree(
                    v-for="stree in clause.strees"
                )
                  .token_id {{ stree.token_id }}
                  .body
                    code {{ stree.token }}
      h5 Parse Cursor
      .cursor(v-if="parsed_state.cursor")
        | {{ parsed_state.cursor }}
</template>

<script lang="ts">

import { reactive, ref, Ref, SetupContext, defineComponent, onMounted, PropType, watch, computed } from '@vue/composition-api';
import _ from "lodash";
import * as Lexer from "@/models/lexer";
import * as Parser from "@/models/parser";
import * as MS from "@/models/minishell";

export default defineComponent({

  setup(prop: {
  }, context: SetupContext) {
    const viewData: {
      line_buffer: string;
      lines: string[];
      line_pointer: number;
    } = reactive({
      line_buffer: "",
      lines: [],
      line_pointer: -1,
    });

    const eventHandlers = {
      readline: () => {
        setTimeout(() => {
          const str = (viewData.line_buffer || "").replaceAll(/\n/g, "") + "\n";
          console.log({ str })
          if (!str.trim()) { return; }
          // line_buffer -> present_line に追加
          viewData.lines.push(str);
          viewData.line_pointer = viewData.lines.length - 1 
          viewData.line_buffer = "";
        }, 10);
      }
    };
    const present_line = computed(() => {
      if (viewData.line_pointer < 0 || viewData.lines.length <= viewData.line_pointer) { return null; }
      return viewData.lines[viewData.line_pointer];
    });
    const present_line_fordisp = computed(() => {
      if (!present_line.value) { return "(none)"; }
      return present_line.value.replaceAll(/\t|\n/g, (c => {
        switch (c) {
          case "\t": return "\\t";
          case "\n": return "\\n";
        }
        return c;
      }));
    });

    /**
     * レキサにより得られたワードの連結リスト
     */
    const lexed_wordlist = computed(() => {
      const line = present_line.value;
      if (!line) { return null; }
      return Lexer.lexer(line);
    });

    /**
     * lexed_wordlist を配列にflattenしたもの
     */
    const lexed_flattened_wordlist = computed(() => {
      const flist: MS.WordList[] = [];
      let wlist = lexed_wordlist.value;
      while (wlist) {
        flist.push({ ...wlist, next: null });
        wlist = wlist.next;
      }
      return flist.slice(1);
    });

    /**
     * パース結果
     */
    const parsed_state = computed(() => {
      const wordlist = lexed_wordlist.value;
      if (!wordlist || !wordlist.next) { return null; }
      const state = Parser.init_parser(wordlist.next);
      Parser.parse(state);
      const flattened_pipelinelist = Parser.flatten_pipelinelist(state.pipelinelist);
      return {
        ...state,
        flattened_pipelinelist,
      };
    });

    /**
     * リダイレクション演算子の説明
     */
    const get_desc_redir_op = (redir: MS.RedirList) => {
      switch (redir.op) {
        case "<": return "INPUT_FROM_FILE";
        case ">": return "OUTPUT_TO_FILE";
        case "<>": return "IN_AND_OUT_FILE";
        case ">>": return "APPEND_TO_FILE";
        case "<&": return "DUP_FOR_INPUT";
        case ">&": return "DUP_FOR_OUTPUT";
        case "<<": return "HEREDOC";
        case "<<-": return "HEREDOC_EXTAB";
      }
      return "* UNEXPECTED *"
    }
    const get_redir_fd = (redir: MS.RedirList) => {
      if (redir.operand_left) {
        return {
          fd: redir.operand_left.token,
          default: false,
        };
      }
      switch (redir.op) {
        case "<":
        case "<<":
        case "<<-":
        case "<&":
        case "<<":
          return {
            fd: "0",
            default: true,
          }
        default:
          return {
            fd: "1",
            default: true,
          }
      }
    }

    return {
      viewData,
      eventHandlers,
      present_line,
      present_line_fordisp,
      lexed_flattened_wordlist,
      parsed_state,

      get_desc_redir_op,
      get_redir_fd,
    };
  }
});
</script>

<style lang="stylus" scoped>
.self
  display flex
  flex-direction column
  position relative
  height 100%
  background-color white
  
  .prompt
    flex-shrink 0
    flex-grow 0
    padding 0.6em
    textarea
      width 100%
      color white
      background-color black
      padding 0.5em
      font-family "Courier New", Consolas, monospace

  .lexer
    flex-shrink 0
    flex-grow 0
    padding 0.6em
    .flattened_list
      display flex
      flex-direction row
      flex-wrap wrap
      .word
        flex-grow 0
        flex-shrink 0
        margin 2px
        padding 0.5em
        border 1px solid black;
        code
          white-space pre-wrap
        .lex_type
          font-size smaller
          font-weight bold
          &.IO_NUMBER
            color red
          &.OPERATOR
            color blue
          &.NEWLINE
            color green

  .parser
    flex-shrink 1
    flex-grow 1
    padding 0.6em

    .state

      .parse_error
        color red
      .parse_unexpexted
        color orange

      .list
        padding 4px
        border 1px solid black
        display flex
        overflow-x scroll
        .andor_list
          padding 4px
          border 1px solid black
          display flex
          margin-left 4px
          .pipeline
            padding 4px
            border 1px solid black
            display flex
            margin-left 4px
            .clause
              padding 4px
              border 1px solid black
              margin-left 4px
              .redirs, .strees
                padding 4px
                display flex
              .redir
                padding 4px
                border 1px solid black
                border-radius 8px
                margin-left 4px
              .stree
                padding 4px
                border 1px solid black
                margin-left 4px

code
  white-space pre-wrap
  font-family "Courier New", Consolas, monospace
  font-weight bold

</style>