<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2014  Avencall
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

$form = &$this->get_module('form');
$dhtml = &$this->get_module('dhtml');

?>
    <h3 class="login-box-msg">
      <?php echo $this->bbf('title_content_name'); ?>
    </h3>
    <hr>
    <form action="#" method="post" accept-charset="utf-8">
    <?php echo $form->hidden(array('name'=> DWHO_SESS_NAME, 'value'=> DWHO_SESS_ID)); ?>
      <div class="form-group">
        <div class="form-group has-feedback">
          <span class="form-control-feedback glyphicon glyphicon-envelope"></span>
          <input class=" form-control" id="it-login" size="20" name="login" type="text" value="<?php echo $this->bbf('fm_login'); ?>">
        </div>
      </div>
    
      <div class="form-group">
        <div class="form-group has-feedback">
          <span class="form-control-feedback glyphicon glyphicon-lock"></span>
          <input class=" form-control" id="it-password" size="20" name="password" type="password" value="<?php echo $this->bbf('fm_password'); ?>">
        </div>
      </div>

      <div class="form-group">
        <select class="form-control" id="it-language" name="language">
        <?php
          foreach($this->get_var('language') as $key => $lang) {
            $selected = '';
            if (DWHO_I18N_BABELFISH_LANGUAGE == $key) { $selected = "selected='selected'"; }
              echo "<option value=$key $selected>$lang</option>";
           }
        ?>
        </select>
      </div>

      <div class="row">
        <div class="col-sm-12 col-sm-offset-4">
          <div>
            <input class="btn btn-primary" id="it-submit" name="submit" type="submit" value="<?php echo $this->bbf('fm_bt-connection'); ?>">
          </div>
        </div>
      </div>
    </form>

<script type="text/javascript">
dwho.dom.set_onload(function () {
    dwho.form.set_events_text_helper('it-login');
    dwho.form.set_events_text_helper('it-password');
});
</script>
