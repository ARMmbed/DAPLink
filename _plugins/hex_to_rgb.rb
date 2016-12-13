# Simple hex conversion Liquid filter plugin
#
# Convert a string of hex values into an array of decimal values.
#
# Examples:
#   {{ "aabbcc" | hex_to_rgb }}
#   # => [170, 187, 204]
#
#   {{ "abc" | hex_to_rgb }}
#   # => [170, 187, 204]
#
#   {{ "0a0b0c" | hex_to_rgb }}
#   # => [10, 11, 12]
#
# hexval - string of valid hexidecimal characters
#
# Returns an array of decimal values for the parse hex characters
#

module Jekyll
  module HexToRGB
    def hex_to_rgb(hexval)
      if hexval.length.even?
        hexval.scan(/../).map {|color| color.to_i(16)}
      else
        hexval.scan(/./).map {|color| (color+color).to_i(16)}
      end
    end
  end
end

Liquid::Template.register_filter(Jekyll::HexToRGB)
