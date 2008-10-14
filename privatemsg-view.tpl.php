<!--pre><?php //print print_r($message, 1); ?></pre-->
<?php 
  //each file loads it's own styles because we cant predict which file will be loaded 
  drupal_add_css(drupal_get_path('module', 'privatemsg').'/styles/privatemsg-view.css');
?>
<div class="privatemsg-box-fb">
  <div class="left-column">
    <div class="avatar-fb">
      <?php print $author_picture; ?>
    </div>
  </div>
  <div class="middle-column">
    <div class="name">
      <?php print $author_name_link; ?>
    </div>
    <div class="date">
      <?php print $message_timestamp; ?>
    </div>
  </div>
  <div class="right-column">
    <div class="message-body">
      <?php print $message_body; ?>
    </div>
    <div class="message-actions">
      <?php print $message_actions; ?>
    </div>
  </div>
  <div class="clear-both bottom-border"></div>
</div>