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
    h4 Lexed: Word List
    .flattened_list(v-if="lexed_flattened_wordlist")
      .word(v-for="word in lexed_flattened_wordlist")
        .start_at start_at: {{ word.i }}
        .lex_type(:class="word.lex_type") {{ word.lex_type }}
        .body
          code {{ word.word }}
  .parser
    h4 Parsed(N/A)
</template>

<script lang="ts">

import { reactive, ref, Ref, SetupContext, defineComponent, onMounted, PropType, watch, computed } from '@vue/composition-api';
import _ from "lodash";
import * as Lexer from "@/models/lexer";
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

    const lexed_wordlist = computed(() => {
      const line = present_line.value;
      if (!line) { return null; }
      return Lexer.lexer(line);
    });

    const lexed_flattened_wordlist = computed(() => {
      const flist: MS.WordList[] = [];
      let wlist = lexed_wordlist.value;
      while (wlist) {
        flist.push({ ...wlist, next: null });
        wlist = wlist.next;
      }
      return flist.slice(1);
    });

    return {
      viewData,
      eventHandlers,
      present_line,
      present_line_fordisp,
      lexed_flattened_wordlist,
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
    .present
      code
        white-space pre-wrap

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
</style>